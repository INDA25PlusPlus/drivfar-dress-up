#include <stdio.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <stdbool.h>

typedef enum { PAGE_DRESS_UP, PAGE_GRADING } Page;

static bool isMouseOverText(sfText *text, sfRenderWindow *window)
{
	sfFloatRect bounds = sfText_getGlobalBounds(text);
	sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
	return sfFloatRect_contains(&bounds, (float)mousePos.x,
				    (float)mousePos.y);
}

int main(void)
{
	sfVideoMode mode = { 800, 600, 32 };
	sfRenderWindow *window = sfRenderWindow_create(
		mode, "Dress Up Skeleton", sfResize | sfClose, NULL);
	if (!window)
		return 1;

	/* Background image */
	sfTexture *backgroundTexture =
		sfTexture_createFromFile("cute_image.jpg", NULL);
	if (!backgroundTexture) {
		sfRenderWindow_destroy(window);
		return 1;
	}

	sfSprite *backgroundSprite = sfSprite_create();
	if (!backgroundSprite) {
		sfTexture_destroy(backgroundTexture);
		sfRenderWindow_destroy(window);
		return 1;
	}
	sfSprite_setTexture(backgroundSprite, backgroundTexture, sfTrue);

	/* Scale background to fit window */
	sfVector2u textureSize = sfTexture_getSize(backgroundTexture);
	if (textureSize.x > 0 && textureSize.y > 0) {
		sfSprite_setScale(backgroundSprite,
				  (sfVector2f){ 800.f / (float)textureSize.x,
						600.f / (float)textureSize.y });
	}

	/* Font */
	sfFont *font = sfFont_createFromFile("arial.ttf");
	if (!font) {
		sfSprite_destroy(backgroundSprite);
		sfTexture_destroy(backgroundTexture);
		sfRenderWindow_destroy(window);
		return 1;
	}

	/* Music */
	sfMusic *music = sfMusic_createFromFile(
		"nice_music.ogg"); /* Morrowind explore track, of course */
	if (!music) {
		sfFont_destroy(font);
		sfSprite_destroy(backgroundSprite);
		sfTexture_destroy(backgroundTexture);
		sfRenderWindow_destroy(window);
		return 1;
	}
	sfMusic_setLoop(music, sfTrue);
	sfMusic_play(music);

	/* Main title */
	sfText *titleText = sfText_create();
	sfText_setFont(titleText, font);
	sfText_setCharacterSize(titleText, 42);
	sfText_setPosition(titleText, (sfVector2f){ 40.f, 30.f });
	sfText_setFillColor(titleText, sfBlack);

	/* Body text */
	sfText *bodyText = sfText_create();
	sfText_setFont(bodyText, font);
	sfText_setCharacterSize(bodyText, 28);
	sfText_setPosition(bodyText, (sfVector2f){ 40.f, 120.f });
	sfText_setFillColor(bodyText, sfBlack);

	/* Right arrow */
	sfText *arrowText = sfText_create();
	sfText_setFont(arrowText, font);
	sfText_setString(arrowText, ">");
	sfText_setCharacterSize(arrowText, 60);
	sfText_setPosition(arrowText, (sfVector2f){ 720.f, 260.f });
	sfText_setFillColor(arrowText, sfBlack);

	/* Left arrow */
	sfText *backArrowText = sfText_create();
	sfText_setFont(backArrowText, font);
	sfText_setString(backArrowText, "<");
	sfText_setCharacterSize(backArrowText, 60);
	sfText_setPosition(backArrowText, (sfVector2f){ 40.f, 260.f });
	sfText_setFillColor(backArrowText, sfBlack);

	Page currentPage = PAGE_DRESS_UP;

	while (sfRenderWindow_isOpen(window)) {
		sfEvent event;
		while (sfRenderWindow_pollEvent(window, &event)) {
			if (event.type == sfEvtClosed)
				sfRenderWindow_close(window);

			if (event.type == sfEvtMouseButtonPressed &&
			    event.mouseButton.button == sfMouseLeft) {
				if (currentPage == PAGE_DRESS_UP &&
				    isMouseOverText(arrowText, window)) {
					currentPage = PAGE_GRADING;
				} else if (currentPage == PAGE_GRADING &&
					   isMouseOverText(backArrowText,
							   window)) {
					currentPage = PAGE_DRESS_UP;
				}
			}
		}

		if (currentPage == PAGE_DRESS_UP) {
			sfText_setString(titleText, "Dress Up Page");
			sfText_setString(bodyText,
					 "Skeleton of the dress up page.\n\n"
					 "- Character display goes here\n"
					 "- Clothing selection goes here\n"
					 "- Color options go here\n\n"
					 "Click the arrow to go to grading.");
		} else {
			sfText_setString(titleText, "Grading Page");
			sfText_setString(bodyText,
					 "Skeleton of the grading page.\n\n"
					 "- Score/result goes here\n"
					 "- Criteria breakdown goes here\n"
					 "- Summary/comment goes here\n\n"
					 "Click the arrow to go back.");
		}

		sfRenderWindow_clear(window, sfBlack);

		/* Draw background first */
		sfRenderWindow_drawSprite(window, backgroundSprite, NULL);

		/* Draw page content */
		sfRenderWindow_drawText(window, titleText, NULL);
		sfRenderWindow_drawText(window, bodyText, NULL);

		if (currentPage == PAGE_DRESS_UP)
			sfRenderWindow_drawText(window, arrowText, NULL);
		else
			sfRenderWindow_drawText(window, backArrowText, NULL);

		sfRenderWindow_display(window);
	}

	sfMusic_destroy(music);
	sfText_destroy(backArrowText);
	sfText_destroy(arrowText);
	sfText_destroy(bodyText);
	sfText_destroy(titleText);
	sfFont_destroy(font);
	sfSprite_destroy(backgroundSprite);
	sfTexture_destroy(backgroundTexture);
	sfRenderWindow_destroy(window);

	return 0;
}