// TODO: Fix me!!
//   Rename RendererData to RenderData
#include <err.h>
#include "clay.h"
#include <CSFML/Graphics.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "clay_render_sfml.h"

static sfColor clayColorToSfColor(Clay_Color color)
{
    return (sfColor){ .r = (uint8_t)color.r,
                      .g = (uint8_t)color.g,
                      .b = (uint8_t)color.b,
                      .a = (uint8_t)color.a };
}

// Global for convenience. Even in 4K this is enough for smooth curves (low
// radius or rect size coupled with no AA or low resolution might make it appear
// as jagged curves).
static int NUM_CIRCLE_SEGMENTS = 16;

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define M_Ef 2.7182818284590452354f
#define M_LOG2Ef 1.4426950408889634074f
#define M_LOG10Ef 0.43429448190325182765f
#define M_LN2f 0.69314718055994530942f
#define M_LN10f 2.30258509299404568402f
#define M_PIf 3.14159265358979323846f
#define M_PI_2f 1.57079632679489661923f
#define M_PI_4f 0.78539816339744830962f
#define M_1_PIf 0.31830988618379067154f
#define M_2_PIf 0.63661977236758134308f
#define M_2_SQRTPIf 1.12837916709551257390f
#define M_SQRT2f 1.41421356237309504880f
#define M_SQRT1_2f 0.70710678118654752440f

typedef enum {
    CORNER_TOP_LEFT = 0,
    CORNER_TOP_RIGHT,
    CORNER_BOTTOM_RIGHT,
    CORNER_BOTTOM_LEFT,
} Corner;

static void Clay_Sfml_RenderFillRect(Clay_SfmlRenderData *rendererData,
                                     const sfFloatRect rect,
                                     const Clay_Color _color)
{
    const sfColor color = clayColorToSfColor(_color);

    sfVertex vertices[4] = {
        { rect.position, color },
        { (sfVector2f){ rect.position.x + rect.size.x, rect.position.y },
          color },
        { (sfVector2f){ rect.position.x, rect.position.y + rect.size.y },
          color },
        { (sfVector2f){ rect.position.x + rect.size.x,
                        rect.position.y + rect.size.y },
          color },
    };

    sfRenderWindow_drawPrimitives(rendererData->window, vertices, 4,
                                  sfTriangleStrip, NULL);
}

/// Helper function for `Clay_Sfml_RenderFillRoundedRect` which computes a
/// vertex part of a rect, with the appropriate normalized texture coordinates
/// for it, based on the size of the drawn rect.
/// @param positionRelative Position of the vertex, relative to the top left
///   corner of `rect`
/// @param color
/// @param rect The rect being drawn.
static sfVertex rectVertex(const sfVector2f positionRelative,
                           const sfColor color, const sfFloatRect rect)
{
    return (sfVertex)
    {
        .position = {rect.position.x + positionRelative.x, rect.position.y + positionRelative.y, },
        .color = color,
        .texCoords = { positionRelative.x / rect.size.x, positionRelative.y / rect.size.y },
    };
}

// All rendering is performed by a single SDL call, avoiding multiple RenderRect + plumbing choice for circles.
static void Clay_Sfml_RenderFillRoundedRect(
    Clay_SfmlRenderData *rendererData, const sfFloatRect rect,
    const Clay_CornerRadius cornerRadius, const sfColor color,
    /// If not null, draws rect using the provided texture, scaled and stretched
    /// to cover the rect, overriding `_color`.
    // TODO: Figure out if it actually overrides `_color` or if it's multiplied
    //   with it.
    const sfTexture *texture)
{
    const float minRadius = MIN(rect.size.x, rect.size.y) / 2.0f;
    const float clampedRadii[4] = {
        [CORNER_TOP_LEFT] = MIN(cornerRadius.topLeft, minRadius),
        [CORNER_TOP_RIGHT] = MIN(cornerRadius.topRight, minRadius),
        [CORNER_BOTTOM_RIGHT] = MIN(cornerRadius.bottomRight, minRadius),
        [CORNER_BOTTOM_LEFT] = MIN(cornerRadius.bottomLeft, minRadius),
    };

    size_t numCircleSegments[4] = {
        [CORNER_TOP_LEFT] = MAX(NUM_CIRCLE_SEGMENTS,
                                (size_t)clampedRadii[CORNER_TOP_LEFT] * 0.5f) /
                            4,
        [CORNER_TOP_RIGHT] =
            MAX(NUM_CIRCLE_SEGMENTS,
                (size_t)clampedRadii[CORNER_TOP_RIGHT] * 0.5f) /
            4,
        [CORNER_BOTTOM_RIGHT] =
            MAX(NUM_CIRCLE_SEGMENTS,
                (size_t)clampedRadii[CORNER_BOTTOM_RIGHT] * 0.5f) /
            4,
        [CORNER_BOTTOM_LEFT] =
            MAX(NUM_CIRCLE_SEGMENTS,
                (size_t)clampedRadii[CORNER_BOTTOM_LEFT] * 0.5f) /
            4,
    };
    if (cornerRadius.topLeft <= 0.0) {
        numCircleSegments[CORNER_TOP_LEFT] = 0;
    }
    if (cornerRadius.topRight <= 0.0) {
        numCircleSegments[CORNER_TOP_RIGHT] = 0;
    }
    if (cornerRadius.bottomRight <= 0.0) {
        numCircleSegments[CORNER_BOTTOM_RIGHT] = 0;
    }
    if (cornerRadius.bottomLeft <= 0.0) {
        numCircleSegments[CORNER_BOTTOM_LEFT] = 0;
    }

    size_t totalVertices = 5 + numCircleSegments[CORNER_TOP_LEFT] +
                           numCircleSegments[CORNER_TOP_RIGHT] +
                           numCircleSegments[CORNER_BOTTOM_LEFT] +
                           numCircleSegments[CORNER_BOTTOM_RIGHT];
    size_t vertexIndex = 0;
    sfVertex vertices[totalVertices];

    // Add vertices in clockwise order starting with the top left corner.
    for (Corner corner = 0; corner < 4; corner++) {
        float clampedRadius = clampedRadii[corner];
        if (clampedRadius >= 0.0) {
            const float stepRadians =
                (M_PIf / 2) / (float)numCircleSegments[corner];

            float centerXRelative;
            float centerYRelative;

            switch (corner) {
            case CORNER_TOP_LEFT: {
                centerXRelative = clampedRadius;
                centerYRelative = clampedRadius;
            } break;
            case CORNER_TOP_RIGHT: {
                centerXRelative = rect.size.x - clampedRadius;
                centerYRelative = clampedRadius;
            } break;
            case CORNER_BOTTOM_RIGHT: {
                centerXRelative = rect.size.x - clampedRadius;
                centerYRelative = rect.size.y - clampedRadius;
            } break;
            case CORNER_BOTTOM_LEFT: {
                centerXRelative = clampedRadius;
                centerYRelative = rect.size.y - clampedRadius;
            } break;
            default: {
                err(101, "unreachable");
            }
            }

            for (size_t i = 0; i < numCircleSegments[corner]; i++) {
                float angle =
                    (float)i * stepRadians + M_PIf / 2.0 * corner + M_PIf;
                vertices[vertexIndex++] = rectVertex(
                    (sfVector2f){ centerXRelative + cosf(angle) * clampedRadius,
                                  centerYRelative +
                                      sinf(angle) * clampedRadius },
                    color, rect);
            }
        }

        // Add the final corner vertex, since there must be `n + 1` vertices for
        // `n` circle segments.
        sfVector2f finalVertexPositionRelative;
        switch (corner) {
        case CORNER_TOP_LEFT: {
            finalVertexPositionRelative.x = clampedRadius;
            finalVertexPositionRelative.y = 0;
        } break;
        case CORNER_TOP_RIGHT: {
            finalVertexPositionRelative.x = rect.size.x;
            finalVertexPositionRelative.y = clampedRadius;
        } break;
        case CORNER_BOTTOM_RIGHT: {
            finalVertexPositionRelative.x = rect.size.x - clampedRadius;
            finalVertexPositionRelative.y = rect.size.y;
        } break;
        case CORNER_BOTTOM_LEFT: {
            finalVertexPositionRelative.x = 0;
            finalVertexPositionRelative.y = rect.size.y - clampedRadius;
        } break;
        }
        vertices[vertexIndex++] =
            rectVertex(finalVertexPositionRelative, color, rect);
    }

    // Add the first vertex a second time to close the fan geometry.
    vertices[vertexIndex++] = rectVertex(
        (sfVector2f){ 0, clampedRadii[CORNER_TOP_LEFT] }, color, rect);

    sfRenderStates renderStates = sfRenderStates_default;
    renderStates.coordinateType = sfCoordinateTypeNormalized;
    renderStates.texture = texture;
    sfRenderWindow_drawPrimitives(rendererData->window, vertices, totalVertices,
                                  sfTriangleFan, &renderStates);
}

// Computes the coordinates for one of the vertices of an arc for one of a
// border's corners. Increasing indices move clockwise. The coordinates are
// relative to the box's top left corner.
static sfVector2f cornerArcGetNthVertex(size_t index, Corner corner,
                                        size_t numCircleSegments,
                                        Clay_CornerRadius cornerRadii,
                                        Clay_BoundingBox rect,
                                        sfVector2f radius)
{
    assert(index <= numCircleSegments);

    float offsetX = 0.0;
    float offsetY = 0.0;
    switch (corner) {
    case CORNER_TOP_LEFT: {
        const float cornerRadius = cornerRadii.topLeft;
        offsetX = cornerRadius;
        offsetY = cornerRadius;
    } break;
    case CORNER_TOP_RIGHT: {
        const float cornerRadius = cornerRadii.topRight;
        offsetX = rect.width - cornerRadius;
        offsetY = cornerRadius;
    } break;
    case CORNER_BOTTOM_RIGHT: {
        const float cornerRadius = cornerRadii.bottomRight;
        offsetX = rect.width - cornerRadius;
        offsetY = rect.height - cornerRadius;
    } break;
    case CORNER_BOTTOM_LEFT: {
        const float cornerRadius = cornerRadii.bottomLeft;
        offsetX = cornerRadius;
        offsetY = rect.height - cornerRadius;
    } break;
    default: {
        err(101, "Unreachable");
    }
    }

    const float stepRadians = (M_PIf / 2) / (float)numCircleSegments;
    const float startAngle = M_PIf + M_PIf * (float)corner / 2.0;

    const float x = cosf(startAngle + stepRadians * index) * radius.x;
    const float y = sinf(startAngle + stepRadians * index) * radius.y;

    return (sfVector2f){ x + offsetX, y + offsetY };
}

static void configureText(const Clay_SfmlRenderData *const renderData,
                          sfText *const text,
                          const Clay_TextRenderData *const config)
{
    char *string = calloc(config->stringContents.length + 1, sizeof(char));
    if (string == NULL) {
        err(1, "render command string allocation failed");
    }
    memcpy(string, config->stringContents.chars, config->stringContents.length);
    sfText_setString(text, string);
    free(string);

    sfFont *font = renderData->fonts[config->fontId];

    sfText_setCharacterSize(text, config->fontSize);

    // We don't need to update the line height property, since clay won't tell
    // us to render more than one line at a time.

    // SFML computes letter spacing as:
    // spaceWidth / 3 * (letterSpacingFactor - 1)
    // This letter spacing value is added to each character's advance. We
    // want the Clay letter spacing property to be measured in pixels.
    const float spaceWidth =
        sfFont_getGlyph(font, U' ', config->fontSize, false, 0.0).advance;
    const float letterSpacingFactor =
        (float)config->letterSpacing * (3.0f / spaceWidth) + 1.0;
    sfText_setLetterSpacing(text, letterSpacingFactor);

    sfText_setFillColor(text, clayColorToSfColor(config->textColor));
}

Clay_Dimensions Clay_Sfml_MeasureText(Clay_StringSlice string,
                                      Clay_TextElementConfig *config,
                                      void *userData)
{
    const Clay_SfmlRenderData *const renderData =
        (Clay_SfmlRenderData *)userData;

    const sfFont *const font = renderData->fonts[config->fontId];
    sfText *const text = sfText_create(font);
    if (text == NULL) {
        err(1, "render command text allocation failed");
    }
    configureText(renderData, text,
                  &(Clay_TextRenderData){
                      .stringContents = string,
                      .textColor = config->textColor,
                      .fontId = config->fontId,
                      .fontSize = config->fontSize,
                      .letterSpacing = config->letterSpacing,
                      .lineHeight = config->lineHeight,
                  });

    sfFloatRect bounds = sfText_getLocalBounds(text);

    return (Clay_Dimensions){
        .width = bounds.size.x + bounds.position.x,
        .height = config->lineHeight == 0 ?
                      sfFont_getLineSpacing(font, config->fontSize) :
                      (float)config->lineHeight,
    };
}

void Clay_Sfml_RenderClayCommands(Clay_SfmlRenderData *rendererData,
                                  Clay_RenderCommandArray *rcommands)
{
    for (size_t i = 0; i < rcommands->length; i++) {
        Clay_RenderCommand *rcmd = Clay_RenderCommandArray_Get(rcommands, i);
        const Clay_BoundingBox boundingBox = rcmd->boundingBox;
        const sfFloatRect rect = { { (int)boundingBox.x, (int)boundingBox.y },
                                   { (int)boundingBox.width,
                                     (int)boundingBox.height } };

        switch (rcmd->commandType) {
        case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
            Clay_RectangleRenderData *config = &rcmd->renderData.rectangle;
            if (config->cornerRadius.topLeft > 0.0 ||
                config->cornerRadius.topRight > 0.0 ||
                config->cornerRadius.bottomRight > 0.0 ||
                config->cornerRadius.bottomLeft > 0.0) {
                Clay_Sfml_RenderFillRoundedRect(
                    rendererData, rect, config->cornerRadius,
                    clayColorToSfColor(config->backgroundColor), NULL);
            } else {
                Clay_Sfml_RenderFillRect(rendererData, rect,
                                         config->backgroundColor);
            }
        } break;
        case CLAY_RENDER_COMMAND_TYPE_TEXT: {
            Clay_TextRenderData *config = &rcmd->renderData.text;
            const sfFont *font = rendererData->fonts[config->fontId];
            sfText *text = sfText_create(font);
            if (text == NULL) {
                err(1, "render command text allocation failed");
            }
            configureText(rendererData, text, config);

            sfText_setPosition(text, rect.position);

            sfRenderWindow_drawText(rendererData->window, text, NULL);
            sfText_destroy(text);
        } break;
        case CLAY_RENDER_COMMAND_TYPE_BORDER: {
            const Clay_BorderRenderData *config = &rcmd->renderData.border;
            const sfColor color = clayColorToSfColor(config->color);

            const float minRadius = MIN(rect.size.x, rect.size.y) / 2.0f;
            const Clay_CornerRadius clampedRadii = {
                .topLeft = MIN(config->cornerRadius.topLeft, minRadius),
                .topRight = MIN(config->cornerRadius.topRight, minRadius),
                .bottomLeft = MIN(config->cornerRadius.bottomLeft, minRadius),
                .bottomRight = MIN(config->cornerRadius.bottomRight, minRadius)
            };

            //increase circle segments for larger circles, 2.0 is arbitrary.
            size_t numArcSegments[4] = {
                [CORNER_TOP_LEFT] = MAX(NUM_CIRCLE_SEGMENTS,
                                        (int)(clampedRadii.topLeft * 2.0f)) /
                                    4,
                [CORNER_TOP_RIGHT] = MAX(NUM_CIRCLE_SEGMENTS,
                                         (int)(clampedRadii.topRight * 2.0f)) /
                                     4,
                [CORNER_BOTTOM_RIGHT] =
                    MAX(NUM_CIRCLE_SEGMENTS,
                        (int)(clampedRadii.bottomRight * 2.0f)) /
                    4,
                [CORNER_BOTTOM_LEFT] =
                    MAX(NUM_CIRCLE_SEGMENTS,
                        (int)(clampedRadii.bottomLeft * 2.0f)) /
                    4,
            };
            if (clampedRadii.topLeft <= 0) {
                numArcSegments[CORNER_TOP_LEFT] = 0;
            }
            if (clampedRadii.topRight <= 0) {
                numArcSegments[CORNER_TOP_RIGHT] = 0;
            }
            if (clampedRadii.bottomRight <= 0) {
                numArcSegments[CORNER_BOTTOM_RIGHT] = 0;
            }
            if (clampedRadii.bottomLeft <= 0) {
                numArcSegments[CORNER_BOTTOM_LEFT] = 0;
            }

            size_t verticesLen =
                8 + 2; // The four corners pairs + an extra pair
            if (clampedRadii.topLeft > 0) {
                verticesLen += numArcSegments[CORNER_TOP_LEFT] * 2;
            }
            if (clampedRadii.topRight > 0) {
                verticesLen += numArcSegments[CORNER_TOP_RIGHT] * 2;
            }
            if (clampedRadii.bottomRight > 0) {
                verticesLen += numArcSegments[CORNER_BOTTOM_RIGHT] * 2;
            }
            if (clampedRadii.bottomLeft > 0) {
                verticesLen += numArcSegments[CORNER_BOTTOM_LEFT] * 2;
            }

            size_t vertexIndex = 0;
            sfVertex vertices[verticesLen];

            // Go through each corner, adding their vertices, alternating the
            // outer and inner arc's vertices.
            for (Corner corner = CORNER_TOP_LEFT; corner < 4; corner++) {
                // Add the corner vertices. There are n + 1 vertices for n
                // circle segments.
                for (size_t i = 0; i < numArcSegments[corner] + 1; i++) {
                    float cornerRadius;
                    sfVector2f cornerRadiusInner;
                    switch (corner) {
                    case CORNER_TOP_LEFT: {
                        cornerRadius = clampedRadii.topLeft;
                        cornerRadiusInner = (sfVector2f){
                            cornerRadius - config->width.left,
                            cornerRadius - config->width.top,
                        };
                    } break;
                    case CORNER_TOP_RIGHT: {
                        cornerRadius = clampedRadii.topRight;
                        cornerRadiusInner = (sfVector2f){
                            cornerRadius - config->width.right,
                            cornerRadius - config->width.top,
                        };
                    } break;
                    case CORNER_BOTTOM_RIGHT: {
                        cornerRadius = clampedRadii.bottomRight;
                        cornerRadiusInner = (sfVector2f){
                            cornerRadius - config->width.right,
                            cornerRadius - config->width.bottom,
                        };
                    } break;
                    case CORNER_BOTTOM_LEFT: {
                        cornerRadius = clampedRadii.bottomLeft;
                        cornerRadiusInner = (sfVector2f){
                            cornerRadius - config->width.left,
                            cornerRadius - config->width.bottom,
                        };
                    } break;
                    default: {
                        err(101, "unreachable");
                    }
                    }

                    // Add the outer arc point.
                    vertices[vertexIndex++] = rectVertex(
                        cornerArcGetNthVertex(i, corner, numArcSegments[corner],
                                              clampedRadii, boundingBox,
                                              (sfVector2f){ cornerRadius,
                                                            cornerRadius }),
                        color, rect);
                    // Add the inner arc point.
                    vertices[vertexIndex++] = rectVertex(
                        cornerArcGetNthVertex(i, corner, numArcSegments[corner],
                                              clampedRadii, boundingBox,
                                              cornerRadiusInner),
                        color, rect);
                }
            }

            // Add the first two vertices of the top left corner again to close
            // the triangle strip.
            // Add the outer arc point.
            {
                vertices[vertexIndex++] = rectVertex(
                    cornerArcGetNthVertex(0, CORNER_TOP_LEFT,
                                          numArcSegments[CORNER_TOP_LEFT],
                                          clampedRadii, boundingBox,
                                          (sfVector2f){ clampedRadii.topLeft,
                                                        clampedRadii.topLeft }),
                    color, rect);
            }
            // Add the inner arc point.
            {
                const float cornerRadius = clampedRadii.topLeft;
                const sfVector2f cornerRadiusInner = (sfVector2f){
                    cornerRadius - config->width.left,
                    cornerRadius - config->width.top,
                };
                vertices[vertexIndex++] = rectVertex(
                    cornerArcGetNthVertex(
                        0, CORNER_TOP_LEFT, numArcSegments[CORNER_TOP_LEFT],
                        clampedRadii, boundingBox, cornerRadiusInner),
                    color, rect);
            }

            assert(vertexIndex == verticesLen);

            sfRenderWindow_drawPrimitives(rendererData->window, vertices,
                                          verticesLen, sfTriangleStrip, NULL);
        } break;
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
            sfVector2u size = sfRenderWindow_getSize(rendererData->window);
            // Is expressed as factors of the window viewport's size.
            sfFloatRect scissor = {
                .position = { boundingBox.x / (float)size.x,
                              boundingBox.y / (float)size.y },
                .size = { boundingBox.width / (float)size.x,
                          boundingBox.height / (float)size.y },
            };
            sfView *view =
                sfView_copy(sfRenderWindow_getView(rendererData->window));
            if (!view) {
                err(1, "Failed allocating view\n");
            }
            sfView_setScissor(view, scissor);
            sfRenderWindow_setView(rendererData->window, view);
            sfView_destroy(view);
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
            sfView *view =
                sfView_copy(sfRenderWindow_getView(rendererData->window));
            if (!view) {
                err(1, "Failed allocating view\n");
            }
            sfView_setScissor(view, (sfFloatRect){ .position = { 0.0, 0.0 },
                                                   .size = { 1.0, 1.0 } });
            sfRenderWindow_setView(rendererData->window, view);
            sfView_destroy(view);
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
            Clay_ImageRenderData *config = &rcmd->renderData.image;
            sfTexture *texture = (sfTexture *)config->imageData;

            // TODO: Change to white/black when I've figured the behavior of texture vertex colors
            sfColor color = (sfColor){ 0xd8, 0x48, 0x1c, 0xff };

            if (config->cornerRadius.topLeft > 0.0 ||
                config->cornerRadius.topRight > 0.0 ||
                config->cornerRadius.bottomRight > 0.0 ||
                config->cornerRadius.bottomLeft > 0.0) {
                Clay_Sfml_RenderFillRoundedRect(
                    rendererData, rect, config->cornerRadius, color, texture);
            } else {
                sfVertex vertices[4] = {
                    { rect.position, color, (sfVector2f){ 0.0, 0.0 } },
                    { (sfVector2f){ rect.position.x + rect.size.x,
                                    rect.position.y },
                      color, (sfVector2f){ 1.0, 0.0 } },
                    { (sfVector2f){ rect.position.x,
                                    rect.position.y + rect.size.y },
                      color, (sfVector2f){ 0.0, 1.0 } },
                    { (sfVector2f){ rect.position.x + rect.size.x,
                                    rect.position.y + rect.size.y },
                      color, (sfVector2f){ 1.0, 1.0 } },
                };

                sfRenderStates renderStates = sfRenderStates_default;
                renderStates.coordinateType = sfCoordinateTypeNormalized;
                renderStates.texture = texture;
                sfRenderWindow_drawPrimitives(rendererData->window, vertices, 4,
                                              sfTriangleFan, &renderStates);
            }

            break;
        }
        default:
            fprintf(stderr, "Unknown render command type: %d",
                    rcmd->commandType);
        }
    }
}