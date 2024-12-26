#define CLAY_IMPLEMENTATION
#include "../../clay.h"
#include "../../renderers/raylib/clay_renderer_raylib.c"

const uint32_t FONT_ID_BODY_24 = 0;

bool killApp = false;

// Basic Error HANDLER
void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
    if (errorData.errorType == CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED) {
        killApp = true;
        Clay_SetMaxElementCount(Clay__maxElementCount * 2);
    } else if (errorData.errorType == CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED) {
        killApp = true;
        Clay_SetMaxMeasureTextCacheWordCount(Clay__maxMeasureTextCacheWordCount * 2);
    }
}

int main(void) {
	Clay_Raylib_Initialize(1280,720,"hello",FLAG_WINDOW_RESIZABLE);

	uint32_t clayRequiredMemory = Clay_MinMemorySize();

	Clay_Arena clay_memory = (Clay_Arena) {
		.capacity = clayRequiredMemory,
		.memory = malloc(clayRequiredMemory)
	};

	Clay_Initialize(clay_memory, (Clay_Dimensions) {
		.width  = GetScreenWidth(),
		.height = GetScreenHeight()
	},(Clay_ErrorHandler)  { HandleClayErrors });

	Clay_SetMeasureTextFunction(Raylib_MeasureText);

	Raylib_fonts[FONT_ID_BODY_24] = (Raylib_Font) {
        .font = LoadFontEx("/home/rlc/.local/share/fonts/Firacode/FiraCodeNerdFont-Medium.ttf",24,0,0),
        .fontId = FONT_ID_BODY_24,
    };
	SetTextureFilter(Raylib_fonts[FONT_ID_BODY_24].font.texture, TEXTURE_FILTER_TRILINEAR);

	Clay_LayoutConfig centred_layout = ( Clay_LayoutConfig ) { 
		.sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW() },
		.layoutDirection = CLAY_TOP_TO_BOTTOM, .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER, } 
	};

	while (!WindowShouldClose()) {
		if (killApp) {
			break;
		}

		Clay_SetLayoutDimensions((Clay_Dimensions) {
			.width  = GetScreenWidth(),
			.height = GetScreenHeight()
		});

		Clay_BeginLayout();

		CLAY(
				CLAY_RECTANGLE({ .color = {45,43,43,255} }),
				CLAY_LAYOUT(centred_layout),
                CLAY_TEXT(
				CLAY_STRING("Hello World"), 
				CLAY_TEXT_CONFIG({ 
					.fontSize = 24, 
					.textColor = {225, 255, 255, 255} ,
					})
				)

		) { }; 


		Clay_RenderCommandArray renderCommandsArray = Clay_EndLayout();



		BeginDrawing();
		ClearBackground(BLACK);
		Clay_Raylib_Render(renderCommandsArray);
		EndDrawing();

	}
	return 0;
}
