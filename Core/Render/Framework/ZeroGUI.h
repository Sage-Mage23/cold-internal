﻿
wchar_t* s2wc(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

namespace ZeroGUI
{
	namespace Input
	{
		bool mouseDown[5];
		bool mouseDownAlready[256];

		bool keysDown[256];
		bool keysDownAlready[256];

		bool IsAnyMouseDown()
		{
			if (mouseDown[0]) return true;
			if (mouseDown[1]) return true;
			if (mouseDown[2]) return true;
			if (mouseDown[3]) return true;
			if (mouseDown[4]) return true;

			return false;
		}

		bool IsMouseClicked(int button, int element_id, bool repeat)
		{
			if (mouseDown[button])
			{
				if (!mouseDownAlready[element_id])
				{
					mouseDownAlready[element_id] = true;
					return true;
				}
				if (repeat)
					return true;
			}
			else
			{
				mouseDownAlready[element_id] = false;
			}
			return false;
		}
		bool IsKeyPressed(int key, bool repeat)
		{
			if (keysDown[key])
			{
				if (!keysDownAlready[key])
				{
					keysDownAlready[key] = true;
					return true;
				}
				if (repeat)
					return true;
			}
			else
			{
				keysDownAlready[key] = false;
			}
			return false;
		}

		void Handle()
		{
		if (CALL(GetAsyncKeyState,0x01))
				mouseDown[0] = true;
			else
				mouseDown[0] = false;
		}
	}
}

namespace ZeroGUI
{
	namespace Colors
	{
		FLinearColor Text = { 1.0f, 1.0f, 1.0f, 1.0f };
		FLinearColor Text_Shadow = { 0.0f, 0.0f, 0.0f, 0.0f };
		FLinearColor Text_Outline = { 0.0f, 0.0f, 0.0f, 0.30f };

		FLinearColor Window_Background = { 0.f, 0.f, 0.f, 1.0f };
		FLinearColor Window_Header = { 1.f, 1.f, 1.f, 1.0f };

		FLinearColor Button_Idle = { 0.2f, 0.2f, 0.2f, 1.0f };
		FLinearColor Button_Hovered = { 0.9f, 0.9f, 0.9f, 1.0f };
		FLinearColor Button_Active = { 1.0f, 1.0f, 1.0f, 1.0f };   // Even brighter red when active

		FLinearColor Checkbox_Idle = { 0.2f, 0.2f, 0.2f, 1.0f };
		FLinearColor Checkbox_Hovered = { 0.9f, 0.9f, 0.9f, 1.0f };
		FLinearColor Checkbox_Enabled = { 1.0f, 1.0f, 1.0f, 1.0f };   // Even brighter red when active

		FLinearColor Combobox_Idle = { 0.2f, 0.2f, 0.2f, 1.0f };
		FLinearColor Combobox_Hovered = { 0.9f, 0.9f, 0.9f, 1.0f };
		FLinearColor Combobox_Active = { 1.0f, 1.0f, 1.0f, 1.0f };   // Even brighter red when active

		FLinearColor Slider_Idle = { 0.10f, 0.10f, 0.10f, 1.0f };
		FLinearColor Slider_Hovered = { 0.15f, 0.15f, 0.15f, 1.0f };
		FLinearColor Slider_Progress = { 1.0f, 1.0f, 1.0f, 1.0f };
		FLinearColor Accent{ 1.f, 0.25f, 0.f,1.f };
		FLinearColor Slider_Button = { 0.50f, 0.10f, 0.00f, 1.0f }; // 0.22f, 0.45f, 0.78f

		FLinearColor ColorPicker_Background = { 0.05f, 0.05f, 0.05f, 1.0f };

		FLinearColor LightPurple = { 0.80f, 0.60f, 0.90f, 1.0f };
		FLinearColor LightPurple_Hovered = { 0.85f, 0.70f, 0.95f, 1.0f };
	}

	namespace PostRenderer
	{
		struct DrawList
		{
			int type = -1; //1 = FilledRect, 2 = TextLeft, 3 = TextCenter, 4 = Draw_Line
			FVector2D pos;
			FVector2D size;
			FLinearColor color;
			const char* name;
			bool outline;

			FVector2D from;
			FVector2D to;
			int thickness;
		};
		DrawList drawlist[128];

		void drawFilledRect(FVector2D pos, float w, float h, FLinearColor color)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 1;
					drawlist[i].pos = pos;
					drawlist[i].size = FVector2D{ w, h };
					drawlist[i].color = color;
					return;
				}
			}
		}
		void TextLeft(const char* name, FVector2D pos, FLinearColor color, bool outline)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 2;
					drawlist[i].name = name;
					drawlist[i].pos = pos;
					drawlist[i].outline = outline;
					drawlist[i].color = color;
					return;
				}
			}
		}
		void TextCenter(const char* name, FVector2D pos, FLinearColor color, bool outline)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 3;
					drawlist[i].name = name;
					drawlist[i].pos = pos;
					drawlist[i].outline = outline;
					drawlist[i].color = color;
					return;
				}
			}
		}
		void Draw_Line(FVector2D from, FVector2D to, int thickness, FLinearColor color)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 4;
					drawlist[i].from = from;
					drawlist[i].to = to;
					drawlist[i].thickness = thickness;
					drawlist[i].color = color;
					return;
				}
			}
		}
	}

	UCanvas* Canvas;
	wchar_t* s2wc(const char* c)
	{
		const size_t cSize = (strlen)(c)+1;
		wchar_t* wc = new wchar_t[cSize];
		(mbstowcs)(wc, c, cSize);

		return wc;
	}

	bool hover_element = false;
	FVector2D menu_pos = FVector2D{ 0, 0 };
	float offset_x = 0.0f;
	float offset_y = 0.0f;

	FVector2D first_element_pos = FVector2D{ 0, 0 };

	FVector2D last_element_pos = FVector2D{ 0, 0 };
	FVector2D last_element_size = FVector2D{ 0, 0 };

	int current_element = -1;
	FVector2D current_element_pos = FVector2D{ 0, 0 };
	FVector2D current_element_size = FVector2D{ 0, 0 };
	int elements_count = 0;

	bool sameLine = false;

	bool pushY = false;
	float pushYvalue = 0.0f;

	void SetupCanvas(UCanvas* _Canvas)
	{
		Canvas = _Canvas;
	}

	FVector2D CursorPos()
	{
		POINT cursorPos;
		(GetCursorPos)(&cursorPos);
		return FVector2D{ (float)cursorPos.x, (float)cursorPos.y };
	}
	bool MouseInZone(FVector2D pos, FVector2D size)
	{
		FVector2D cursor_pos = CursorPos();

		if (cursor_pos.X > pos.X && cursor_pos.Y > pos.Y)
			if (cursor_pos.X < pos.X + size.X && cursor_pos.Y < pos.Y + size.Y)
				return true;

		return false;
	}

	void SameLine()
	{
		sameLine = true;
	}
	void PushNextElementY(float y, bool from_last_element = true)
	{
		pushY = true;
		if (from_last_element)
			pushYvalue = last_element_pos.Y + last_element_size.Y + y;
		else
			pushYvalue = y;
	}
	void NextColumn(float x)
	{
		offset_x = x;
		PushNextElementY(first_element_pos.Y, false);
	}
	void ClearFirstPos()
	{
		first_element_pos = FVector2D{ 0, 0 };
	}

	void TextLeft(const char* name, FVector2D pos, FLinearColor color, bool outline)
	{
		int length = (strlen)(name)+1;
		Canvas->K2_DrawText(FString{ s2wc(name) }, FVector2D{ pos.X + 1, pos.Y + 1 }, 10.0f, color, false, true, true);
	}
	void TextCenter(const char* name, FVector2D pos, FLinearColor color, bool outline)
	{
		int length = (strlen)(name)+1;
		Canvas->K2_DrawText(FString{ s2wc(name) }, FVector2D{ pos.X + 1, pos.Y + 1 }, 10.0f, color, true, true, true);
	}



	void Draw_Line(FVector2D from, FVector2D to, int thickness, FLinearColor color)
	{
		Canvas->K2_DrawLine(FVector2D{ from.X, from.Y }, FVector2D{ to.X, to.Y }, thickness, color);
	}
	void drawFilledRect(FVector2D initial_pos, float w, float h, FLinearColor color)
	{
		for (float i = 0.0f; i < h; i += 1.0f)
			Canvas->K2_DrawLine(FVector2D{ initial_pos.X, initial_pos.Y + i }, FVector2D{ initial_pos.X + w, initial_pos.Y + i }, 1.0f, color);
	}
	void drawFilledRect_trans(FVector2D initial_pos, float w, float h, FLinearColor color)
	{
		for (float i = 0.0f; i < h; i += 1.0f)
			Canvas->K2_DrawLine(FVector2D{ initial_pos.X, initial_pos.Y + i }, FVector2D{ initial_pos.X + w, initial_pos.Y + i }, 0.4f, color);
	}
	void DrawFilledCircle(FVector2D pos, float r, FLinearColor color)
	{
		float smooth = 0.07f;

		double PI = 3.14159265359;
		int size = (int)(2.0f * PI / smooth) + 1;

		float angle = 0.0f;
		int i = 0;

		for (; angle < 2 * PI; angle += smooth, i++)
		{
			Draw_Line(FVector2D{ pos.X, pos.Y }, FVector2D{ pos.X + cosf(angle) * r, pos.Y + sinf(angle) * r }, 1.0f, color);
		}
	}
	void DrawCircle(FVector2D pos, int radius, int numSides, FLinearColor Color)
	{
		float PI = 3.1415927f;

		float Step = PI * 2.0 / numSides;
		int Count = 0;
		FVector2D V[128];
		for (float a = 0; a < PI * 2.0; a += Step) {
			float X1 = radius * cos(a) + pos.X;
			float Y1 = radius * sin(a) + pos.Y;
			float X2 = radius * cos(a + Step) + pos.X;
			float Y2 = radius * sin(a + Step) + pos.Y;
			V[Count].X = X1;
			V[Count].Y = Y1;
			V[Count + 1].X = X2;
			V[Count + 1].Y = Y2;
			//Draw_Line(FVector2D{ pos.X, pos.Y }, FVector2D{ X2, Y2 }, 1.0f, Color); // Points from Centre to ends of circle
			Draw_Line(FVector2D{ V[Count].X, V[Count].Y }, FVector2D{ X2, Y2 }, 1.0f, Color);// Circle Around
		}
	}

	FVector2D dragPos;
	bool Window(const char* name, FVector2D* pos, FVector2D size, bool isOpen, float width, float height)
	{
		elements_count = 0;

		if (!isOpen)
			return false;

		bool isHovered = MouseInZone(FVector2D{ pos->X, pos->Y }, size);

		//Drop last element
		if (current_element != -1 && !CALL(GetAsyncKeyState, 0x1))
		{
			current_element = -1;
		}

		//Drag
		if (hover_element && CALL(GetAsyncKeyState, 0x1))
		{

		}
		else if ((isHovered || dragPos.X != 0) && !hover_element)
		{
			if (Input::IsMouseClicked(0, elements_count, true))
			{
				FVector2D cursorPos = CursorPos();

				cursorPos.X -= size.X;
				cursorPos.Y -= size.Y;

				if (dragPos == 0)
				{
					dragPos.X = (cursorPos.X - pos->X);
					dragPos.Y = (cursorPos.Y - pos->Y);
				}
				pos->X = cursorPos.X - dragPos.X;
				pos->Y = cursorPos.Y - dragPos.Y;
			}
			else
			{
				dragPos = FVector2D{ 0, 0 };
			}
		}
		else
		{
			hover_element = false;
		}


		offset_x = 0.0f; offset_y = 0.0f;
		menu_pos = FVector2D{ pos->X, pos->Y };
		first_element_pos = FVector2D{ 0, 0 };
		current_element_pos = FVector2D{ 0, 0 };
		current_element_size = FVector2D{ 0, 0 };

		// Dark transparent background
		//drawFilledRect_trans(FVector2D{ 0, 0 }, width, height, FLinearColor{ 0.0f, 0.0f, 0.0f, 0.4f });


		// Bg
		drawFilledRect(FVector2D{ pos->X, pos->Y }, size.X, size.Y, Colors::Window_Background);
		//drawFilledRect(FVector2D{ pos->X, pos->Y }, 122, size.Y, FLinearColor{ 0.006f, 0.006f, 0.006f, 1.0f });//My tabs bg

		// Header
		drawFilledRect(FVector2D{ pos->X, pos->Y }, size.X, 25.0f, Colors::Window_Header);

		offset_y += 25.0f;

		// Title
		FVector2D titlePos = FVector2D{ pos->X + size.X / 2, pos->Y + 25 / 2 };
		TextCenter(name, titlePos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

		return true;
	}

	void Text(const char* text, bool center = false, bool outline = false)
	{
		elements_count++;

		float size = 25;
		FVector2D padding = FVector2D{ 10, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}

		if (!sameLine)
			offset_y += size + padding.Y;

		//Text
		FVector2D textPos = FVector2D{ pos.X + 5.0f, pos.Y + size / 2 };
		if (center)
			TextCenter(text, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, outline);
		else
			TextLeft(text, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, outline);

		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}

	bool ButtonTab(const char* name, FVector2D size, bool active)
	{
		elements_count++;

		FVector2D padding = { 5, 10 };
		FVector2D pos = { menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };

		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}

		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}

		bool isHovered = MouseInZone(pos, size);

		// Background
		FLinearColor buttonColor = active ? Colors::Button_Active : (isHovered ? Colors::Button_Hovered : Colors::Button_Idle);
		drawFilledRect(pos, size.X, size.Y, buttonColor);

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		// Text
		FVector2D textPos = { pos.X + size.X / 2, pos.Y + size.Y / 2 };
		TextCenter(name, textPos, { 1.0f, 1.0f, 1.0f, 1.0f }, false);

		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;

		if (isHovered && Input::IsMouseClicked(0, elements_count, false))
			return true;

		return false;
	}

	bool Button(const char* name, FVector2D size)
	{
		elements_count++;

		FVector2D padding = FVector2D{ 5, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

		//Bg
		if (isHovered)
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Idle);
		}

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		//Text
		FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
		//if (!TextOverlapedFromActiveElement(textPos))
		TextCenter(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;

		if (isHovered && Input::IsMouseClicked(0, elements_count, false))
			return true;

		return false;
	}
	void Checkbox(const char* name, bool* value)
	{
		elements_count++;

		float size = 18;
		FVector2D padding = FVector2D{ 10, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, FVector2D{ size, size });

		//Bg
		if (isHovered)
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size, size, Colors::Checkbox_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size, size, Colors::Checkbox_Idle);
		}

		if (!sameLine)
			offset_y += size + padding.Y;

		if (*value)
		{
			drawFilledRect(FVector2D{ pos.X + 3, pos.Y + 3 }, size - 6, size - 6, Colors::Checkbox_Enabled);
			//drawFilledRect(FVector2D{ pos.X + 9, pos.Y + 9 }, size - 18, size - 18, Colors::Checkbox_Hovered);
		}



		//Text
		FVector2D textPos = FVector2D{ pos.X + size + 5.0f, pos.Y + size / 2 };
		//if (!TextOverlapedFromActiveElement(textPos))
		TextLeft(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);


		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;

		if (isHovered && Input::IsMouseClicked(0, elements_count, false))
			*value = !*value;
	}
	void SliderInt( const char* name, int* value, int min, int max )
	{
		elements_count++;

		FVector2D size = FVector2D { 240, 50 };
		FVector2D slider_size = FVector2D { 200, 10 };
		FVector2D padding = FVector2D { 10, 15 };
		FVector2D pos = FVector2D { menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if ( sameLine )
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if ( pushY )
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone( FVector2D { pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size );

		if ( !sameLine )
			offset_y += size.Y + padding.Y;

		//Bg
		if ( isHovered || current_element == elements_count )
		{
			//Drag
			if ( Input::IsMouseClicked( 0, elements_count, true ) )
			{
				current_element = elements_count;

				FVector2D cursorPos = CursorPos( );
				*value = ((cursorPos.X - pos.X) * ((max - min) / slider_size.X)) + min;
				if ( *value < min ) *value = min;
				if ( *value > max ) *value = max;
			}

			drawFilledRect( FVector2D { pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Hovered );
			drawFilledRect( FVector2D { pos.X, pos.Y + slider_size.Y + padding.Y + 5.0f }, 5.0f, 5.0f, Colors::Slider_Progress );

			hover_element = true;
		}
		else
		{
			drawFilledRect( FVector2D { pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Idle );
			drawFilledRect( FVector2D { pos.X, pos.Y + slider_size.Y + padding.Y + 5.0f }, 5.0f, 5.0f, Colors::Slider_Progress );
		}


		//Value
		float oneP = slider_size.X / (max - min);
		drawFilledRect( FVector2D { pos.X, pos.Y + slider_size.Y + padding.Y }, oneP * (*value - min), slider_size.Y, Colors::Slider_Progress );
		//drawFilledRect(FVector2D{ pos.X + oneP * (*value - min) - 10.0f, pos.Y + slider_size.Y - 5.0f + padding.Y }, 20.0f, 20.0f, Colors::Slider_Button);
		DrawFilledCircle( FVector2D { pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 3.3f + padding.Y }, 10.0f, Colors::Slider_Button );
		DrawFilledCircle( FVector2D { pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 3.3f + padding.Y }, 5.0f, Colors::Slider_Progress );

		char buffer [ 32 ];
		sprintf_s( buffer, "%i", *value );
		FVector2D valuePos = FVector2D { pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 25 + padding.Y };
		TextCenter( buffer, valuePos, FLinearColor { 1.0f, 1.0f, 1.0f, 1.0f }, false );

		//Text
		FVector2D textPos = FVector2D { pos.X + 5, pos.Y + 10 };
		TextLeft( name, textPos, FLinearColor { 1.0f, 1.0f, 1.0f, 1.0f }, false );


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if ( first_element_pos.X == 0.0f )
			first_element_pos = pos;
	}
	void SliderFloat(const char* name, float* value, float min, float max, const char* format = "%.0f")
	{
		elements_count++;

		FVector2D size = { 180, 40 };
		FVector2D slider_size = { 140, 7 };
		FVector2D adjust_zone = { 0, 20 };
		FVector2D padding = { 10, 15 };
		FVector2D pos = { menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };

		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}

		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}

		bool isHovered = MouseInZone(pos, { slider_size.X, slider_size.Y + adjust_zone.Y * 1.5f });

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		// Background
		FLinearColor sliderIdleColor = isHovered || current_element == elements_count ? Colors::Slider_Hovered : Colors::Slider_Idle;
		drawFilledRect({ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, sliderIdleColor);
		DrawFilledCircle({ pos.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Progress);
		DrawFilledCircle({ pos.X + slider_size.X, pos.Y + padding.Y + 9.3f }, 3.1f, sliderIdleColor);

		if (isHovered || current_element == elements_count)
		{
			// Drag
			if (Input::IsMouseClicked(0, elements_count, true))
			{
				current_element = elements_count;

				FVector2D cursorPos = CursorPos();
				*value = ((cursorPos.X - pos.X) * ((max - min) / slider_size.X)) + min;
				if (*value < min)
					*value = min;
				if (*value > max)
					*value = max;
			}

			hover_element = true;
		}

		// Text
		FVector2D textPos = { pos.X, pos.Y + 5 };
		TextLeft(name, textPos, Colors::Text, false);

		// Value
		float oneP = slider_size.X / (max - min);
		drawFilledRect({ pos.X, pos.Y + slider_size.Y + padding.Y }, oneP * (*value - min), slider_size.Y, Colors::Slider_Progress);
		DrawFilledCircle({ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 2.66f + padding.Y }, 8.0f, Colors::Slider_Button);
		DrawFilledCircle({ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 2.66f + padding.Y }, 4.0f, Colors::Slider_Progress);

		char buffer[32];
		sprintf_s(buffer, format, *value);
		FVector2D valuePos = { pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 20 + padding.Y };
		TextCenter(buffer, valuePos, Colors::Text, false);

		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}

	void SliderDouble(const char* name, double* value, float min, float max, const char* format = "%.0f")
	{
		elements_count++;

		FVector2D size = { 210, 40 };
		FVector2D slider_size = { 170, 7 };
		FVector2D adjust_zone = { 0, 20 };
		FVector2D padding = { 10, 15 };
		FVector2D pos = { menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };

		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}

		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}

		bool isHovered = MouseInZone(pos, { slider_size.X, slider_size.Y + adjust_zone.Y * 1.5f });

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		// Background
		FLinearColor sliderIdleColor = isHovered || current_element == elements_count ? Colors::Slider_Hovered : Colors::Slider_Idle;
		drawFilledRect({ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, sliderIdleColor);
		DrawFilledCircle({ pos.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Progress);
		DrawFilledCircle({ pos.X + slider_size.X, pos.Y + padding.Y + 9.3f }, 3.1f, sliderIdleColor);

		if (isHovered || current_element == elements_count)
		{
			// Drag
			if (Input::IsMouseClicked(0, elements_count, true))
			{
				current_element = elements_count;

				FVector2D cursorPos = CursorPos();
				*value = ((cursorPos.X - pos.X) * ((max - min) / slider_size.X)) + min;
				if (*value < min)
					*value = min;
				if (*value > max)
					*value = max;
			}

			hover_element = true;
		}

		// Text
		FVector2D textPos = { pos.X, pos.Y + 5 };
		TextLeft(name, textPos, Colors::Text, false);

		// Value
		float oneP = slider_size.X / (max - min);
		drawFilledRect({ pos.X, pos.Y + slider_size.Y + padding.Y }, oneP * (*value - min), slider_size.Y, Colors::Slider_Progress);
		DrawFilledCircle({ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 2.66f + padding.Y }, 8.0f, Colors::Slider_Button);
		DrawFilledCircle({ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 2.66f + padding.Y }, 4.0f, Colors::Slider_Progress);

		char buffer[32];
		sprintf_s(buffer, format, *value);
		FVector2D valuePos = { pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 20 + padding.Y };
		TextCenter(buffer, valuePos, Colors::Text, false);

		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}

	bool checkbox_enabled[256];
	void Combobox(const char* name, FVector2D size, int* value, const char* arg, ...)
	{
		elements_count++;

		FVector2D padding = FVector2D{ 5, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

		//Bg
		if (isHovered || checkbox_enabled[elements_count])
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Combobox_Hovered);

			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Combobox_Idle);
		}

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		//Text
		FVector2D textPos = FVector2D{ pos.X + size.X + 5.0f, pos.Y + size.Y / 2 };
		TextLeft(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

		//Elements
		bool isHovered2 = false;
		FVector2D element_pos = pos;
		int num = 0;

		if (checkbox_enabled[elements_count])
		{
			current_element_size.X = element_pos.X - 5.0f;
			current_element_size.Y = element_pos.Y - 5.0f;
		}
		va_list arguments;
		for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char*))
		{
			//Selected Element
			if (num == *value)
			{
				FVector2D _textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
				TextCenter((char*)arg, _textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
			}

			if (checkbox_enabled[elements_count])
			{
				element_pos.Y += 25.0f;

				isHovered2 = MouseInZone(FVector2D{ element_pos.X, element_pos.Y }, FVector2D{ size.X, 25.0f });
				if (isHovered2)
				{
					hover_element = true;
					PostRenderer::drawFilledRect(FVector2D{ element_pos.X, element_pos.Y }, size.X, 25.0f, Colors::Combobox_Hovered);

					//Click
					if (Input::IsMouseClicked(0, elements_count, false))
					{
						*value = num;
						checkbox_enabled[elements_count] = false;
					}
				}
				else
				{
					PostRenderer::drawFilledRect(FVector2D{ element_pos.X, element_pos.Y }, size.X, 25.0f, Colors::Combobox_Idle);
				}

				PostRenderer::TextLeft((char*)arg, FVector2D{ element_pos.X + 5.0f, element_pos.Y + 15.0f }, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
			}
			num++;
		}
		va_end(arguments);
		if (checkbox_enabled[elements_count])
		{
			current_element_size.X = element_pos.X + 5.0f;
			current_element_size.Y = element_pos.Y + 5.0f;
		}


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;

		if (isHovered && Input::IsMouseClicked(0, elements_count, false))
		{
			checkbox_enabled[elements_count] = !checkbox_enabled[elements_count];
		}
		if (!isHovered && !isHovered2 && Input::IsMouseClicked(0, elements_count, false))
		{
			checkbox_enabled[elements_count] = false;
		}
	}

	int active_hotkey = -1;
	bool already_pressed = false;
	std::string VirtualKeyCodeToString(UCHAR virtualKey)
	{
		UINT scanCode = (MapVirtualKey)(virtualKey, MAPVK_VK_TO_VSC);

		if (virtualKey == VK_LBUTTON)
		{
			return ("Left Mouse Button");
		}
		if (virtualKey == VK_RBUTTON)
		{
			return ("Right Mouse Button");
		}
		if (virtualKey == VK_MBUTTON)
		{
			return ("Middle Mouse Button");
		}
		if (virtualKey == VK_XBUTTON1)
		{
			return ("Side Button 1");
		}
		if (virtualKey == VK_XBUTTON2)
		{
			return ("Side Button 2");
		}

		CHAR szName[128];
		int result = 0;
		switch (virtualKey)
		{
		case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
		case VK_RCONTROL: case VK_RMENU:
		case VK_LWIN: case VK_RWIN: case VK_APPS:
		case VK_PRIOR: case VK_NEXT:
		case VK_END: case VK_HOME:
		case VK_INSERT: case VK_DELETE:
		case VK_DIVIDE:
		case VK_NUMLOCK:
			scanCode |= KF_EXTENDED;
		default:
			result = (GetKeyNameTextA)(scanCode << 16, szName, 128);
		}

		return szName;
	}

	int active_hotkey2 = -1; // Separate active key state for Hotkey2
	bool already_pressed2 = false; // S

	void Hotkey2(const char* name, FVector2D size, int* key)
	{
		elements_count++; // Keep this shared as it tracks all elements

		FVector2D padding2 = FVector2D{ 5, 10 };
		FVector2D pos2 = FVector2D{ menu_pos.X + padding2.X + offset_x, menu_pos.Y + padding2.Y + offset_y };
		if (sameLine)
		{
			pos2.X = last_element_pos.X + last_element_size.X + padding2.X;
			pos2.Y = last_element_pos.Y + (last_element_size.Y / 2) - size.Y / 2;
		}
		if (pushY)
		{
			pos2.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos2.Y - menu_pos.Y;
		}
		bool isHovered2 = MouseInZone(FVector2D{ pos2.X, pos2.Y }, size);

		// Background
		if (isHovered2)
		{
			drawFilledRect(FVector2D{ pos2.X, pos2.Y }, size.X, size.Y, Colors::Button_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos2.X, pos2.Y }, size.X, size.Y, Colors::Button_Idle);
		}

		if (!sameLine)
			offset_y += size.Y + padding2.Y;

		if (active_hotkey2 == elements_count) // Use a separate active_hotkey2 variable
		{
			// Text
			FVector2D textPos2 = FVector2D{ pos2.X + size.X / 2, pos2.Y + size.Y / 2 };
			TextCenter("[Press Key]", textPos2, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			if (!ZeroGUI::Input::IsAnyMouseDown())
			{
				already_pressed2 = false; // Use a separate already_pressed2 variable
			}

			if (!already_pressed2)
			{
				for (int code = 0; code < 255; code++)
				{
					if (CALL(GetAsyncKeyState, code))
					{
						*key = code;
						active_hotkey2 = -1;
					}
				}
			}
		}
		else
		{
			// Text
			FVector2D textPos2 = FVector2D{ pos2.X + size.X / 2, pos2.Y + size.Y / 2 };
			TextCenter((char*)VirtualKeyCodeToString(*key).c_str(), textPos2, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			if (isHovered2)
			{
				if (Input::IsMouseClicked(0, elements_count, false))
				{
					already_pressed2 = true;
					active_hotkey2 = elements_count;

					// Queue Fix
					for (int code = 0; code < 255; code++)
						if (CALL(GetAsyncKeyState, code)) {}
				}
			}
			else
			{
				if (Input::IsMouseClicked(0, elements_count, false))
				{
					active_hotkey2 = -1;
				}
			}
		}

		sameLine = false;
		last_element_pos = pos2;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos2;
	}


	void Hotkey(const char* name, FVector2D size, int* key)
	{
		elements_count++;

		FVector2D padding = FVector2D{ 5, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y + (last_element_size.Y / 2) - size.Y / 2;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

		//Bg
		if (isHovered)
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Idle);
		}

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		if (active_hotkey == elements_count)
		{
			//Text
			FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
			TextCenter("[Press Key]", textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			if (!ZeroGUI::Input::IsAnyMouseDown())
			{
				already_pressed = false;
			}

			if (!already_pressed)
			{
				for (int code = 0; code < 255; code++)
				{
					if (CALL(GetAsyncKeyState, code))
					{
						*key = code;
						active_hotkey = -1;
					}
				}
			}
		}
		else
		{
			//Text
			FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
			TextCenter((char*)VirtualKeyCodeToString(*key).c_str(), textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			if (isHovered)
			{
				if (Input::IsMouseClicked(0, elements_count, false))
				{
					already_pressed = true;
					active_hotkey = elements_count;

					//Queue Fix
					for (int code = 0; code < 255; code++)
						if (CALL(GetAsyncKeyState, code)) {}
				}
			}
			else
			{
				if (Input::IsMouseClicked(0, elements_count, false))
				{
					active_hotkey = -1;
				}
			}
		}


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}

	int active_picker = -1;
	FLinearColor saved_color;
	bool ColorPixel(FVector2D pos, FVector2D size, FLinearColor* original, FLinearColor color)
	{
		PostRenderer::drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, color);

		//Выбранный цвет
		if (original->R == color.R && original->G == color.G && original->B == color.B)
		{
			PostRenderer::Draw_Line(FVector2D{ pos.X, pos.Y }, FVector2D{ pos.X + size.X - 1, pos.Y }, 1.0f, FLinearColor{ 0.0f, 0.0f, 0.0f, 1.0f });
			PostRenderer::Draw_Line(FVector2D{ pos.X, pos.Y + size.Y - 1 }, FVector2D{ pos.X + size.X - 1, pos.Y + size.Y - 1 }, 1.0f, FLinearColor{ 0.0f, 0.0f, 0.0f, 1.0f });
			PostRenderer::Draw_Line(FVector2D{ pos.X, pos.Y }, FVector2D{ pos.X, pos.Y + size.Y - 1 }, 1.0f, FLinearColor{ 0.0f, 0.0f, 0.0f, 1.0f });
			PostRenderer::Draw_Line(FVector2D{ pos.X + size.X - 1, pos.Y }, FVector2D{ pos.X + size.X - 1, pos.Y + size.Y - 1 }, 1.0f, FLinearColor{ 0.0f, 0.0f, 0.0f, 1.0f });
		}

		//Смена цвета
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);
		if (isHovered)
		{
			if (Input::IsMouseClicked(0, elements_count, false))
				*original = color;
		}

		return true;
	}
	void GetColor(FLinearColor* color, float* r, float* g, float* b, float* a)
	{
		*r = color->R;
		*g = color->G;
		*b = color->B;
		*a = color->A;
	}
	UINT32 GetColorUINT(int r, int g, int b, int a)
	{
		UINT32 result = (BYTE(a) << 24) + (BYTE(r) << 16) + (BYTE(g) << 8) + BYTE(b);
		return result;
	}

	void SliderAimkey(char* name, int* value, int min, int max) {
		elements_count++;

		FVector2D size = FVector2D{ 230, 25 };
		FVector2D slider_size = FVector2D{ 200, 5 };
		FVector2D adjust_zone = FVector2D{ 0, 20 };
		FVector2D padding = FVector2D{ 10, 12 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x + 9, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y - adjust_zone.Y }, FVector2D{ slider_size.X, slider_size.Y + adjust_zone.Y * 1.5f });

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		//Bg
		if (isHovered || current_element == elements_count)
		{
			//Drag
			if (Input::IsMouseClicked(0, elements_count, true))
			{
				current_element = elements_count;

				FVector2D cursorPos = CursorPos();
				*value = ((cursorPos.X - pos.X) * ((max - min) / slider_size.X)) + min;
				if (*value < min) *value = min;
				if (*value > max) *value = max;
			}

			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y + 4 }, slider_size.X, slider_size.Y, Colors::Slider_Hovered);

			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y + 4 }, slider_size.X, slider_size.Y, Colors::Slider_Idle);
		}


		char buffer[32];
		switch (*value) {
		case 0:
			sprintf_s(buffer, Encrypt("%s : Right Mouse"), name);
			break;
		case 1:
			sprintf_s(buffer, Encrypt("%s : Left Mouse"), name);
			break;
		case 2:
			sprintf_s(buffer, Encrypt("%s : Left Shift"), name);
			break;
		case 3:
			sprintf_s(buffer, Encrypt("%s : Mouse 1"), name);
			break;
		case 4:
			sprintf_s(buffer, Encrypt("%s : Mouse 2"), name);
			break;
		}

		FVector2D textPos = FVector2D{ pos.X, pos.Y - 6 };
		TextLeft(buffer, textPos, Colors::Text, false);

		//Value
		float oneP = slider_size.X / (max - min);
		drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y + 4 }, oneP * (*value - min), slider_size.Y, Colors::Slider_Progress);
		Canvas->K2_DrawLine({ (pos.X + oneP * (*value - min)), (pos.Y + slider_size.Y + 2.66f + padding.Y + 4) + 3 }, { (pos.X + oneP * (*value - min)), (pos.Y + slider_size.Y + 2.66f + padding.Y + 4) - 4 }, 4.f, Colors::Accent);

		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}

	void ColorPicker(const char* name, FLinearColor* color)
	{
		elements_count++;

		float size = 25;
		FVector2D padding = FVector2D{ 10, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, FVector2D{ size, size });

		if (!sameLine)
			offset_y += size + padding.Y;

		if (active_picker == elements_count)
		{
			hover_element = true;

			float sizePickerX = 250;
			float sizePickerY = 250;
			bool isHoveredPicker = MouseInZone(FVector2D{ pos.X, pos.Y }, FVector2D{ sizePickerX, sizePickerY - 60 });

			//Background
			PostRenderer::drawFilledRect(FVector2D{ pos.X, pos.Y }, sizePickerX, sizePickerY - 65, Colors::ColorPicker_Background);

			FVector2D pixelSize = FVector2D{ sizePickerX / 12, sizePickerY / 12 };

			//0
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 174 / 255.f, 235 / 255.f, 253 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 136 / 255.f, 225 / 255.f, 251 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 108 / 255.f, 213 / 255.f, 250 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 89 / 255.f, 175 / 255.f, 213 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 76 / 255.f, 151 / 255.f, 177 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 60 / 255.f, 118 / 255.f, 140 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 43 / 255.f, 85 / 255.f, 100 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 32 / 255.f, 62 / 255.f, 74 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.0f });
			}
			//1
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 175 / 255.f, 205 / 255.f, 252 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 132 / 255.f, 179 / 255.f, 252 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 90 / 255.f, 152 / 255.f, 250 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 55 / 255.f, 120 / 255.f, 250 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 49 / 255.f, 105 / 255.f, 209 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 38 / 255.f, 83 / 255.f, 165 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 28 / 255.f, 61 / 255.f, 120 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 20 / 255.f, 43 / 255.f, 86 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 247 / 255.f, 247 / 255.f, 247 / 255.f, 1.0f });
			}
			//2
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 153 / 255.f, 139 / 255.f, 250 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 101 / 255.f, 79 / 255.f, 249 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 64 / 255.f, 50 / 255.f, 230 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 54 / 255.f, 38 / 255.f, 175 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 39 / 255.f, 31 / 255.f, 144 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 32 / 255.f, 25 / 255.f, 116 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 21 / 255.f, 18 / 255.f, 82 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 16 / 255.f, 13 / 255.f, 61 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 228 / 255.f, 228 / 255.f, 228 / 255.f, 1.0f });
			}
			//3
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 194 / 255.f, 144 / 255.f, 251 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 165 / 255.f, 87 / 255.f, 249 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 142 / 255.f, 57 / 255.f, 239 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 116 / 255.f, 45 / 255.f, 184 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 92 / 255.f, 37 / 255.f, 154 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 73 / 255.f, 29 / 255.f, 121 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 53 / 255.f, 21 / 255.f, 88 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 37 / 255.f, 15 / 255.f, 63 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 203 / 255.f, 203 / 255.f, 203 / 255.f, 1.0f });
			}
			//4
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 224 / 255.f, 162 / 255.f, 197 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 210 / 255.f, 112 / 255.f, 166 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 199 / 255.f, 62 / 255.f, 135 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 159 / 255.f, 49 / 255.f, 105 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 132 / 255.f, 41 / 255.f, 89 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 104 / 255.f, 32 / 255.f, 71 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 75 / 255.f, 24 / 255.f, 51 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 54 / 255.f, 14 / 255.f, 36 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 175 / 255.f, 175 / 255.f, 175 / 255.f, 1.0f });
			}
			//5
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 235 / 255.f, 175 / 255.f, 176 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 227 / 255.f, 133 / 255.f, 135 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 219 / 255.f, 87 / 255.f, 88 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 215 / 255.f, 50 / 255.f, 36 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 187 / 255.f, 25 / 255.f, 7 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 149 / 255.f, 20 / 255.f, 6 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 107 / 255.f, 14 / 255.f, 4 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 77 / 255.f, 9 / 255.f, 3 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 144 / 255.f, 144 / 255.f, 144 / 255.f, 1.0f });
			}
			//6
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 241 / 255.f, 187 / 255.f, 171 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 234 / 255.f, 151 / 255.f, 126 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 229 / 255.f, 115 / 255.f, 76 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 227 / 255.f, 82 / 255.f, 24 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 190 / 255.f, 61 / 255.f, 15 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 150 / 255.f, 48 / 255.f, 12 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 107 / 255.f, 34 / 255.f, 8 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 79 / 255.f, 25 / 255.f, 6 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 113 / 255.f, 113 / 255.f, 113 / 255.f, 1.0f });
			}
			//7
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 245 / 255.f, 207 / 255.f, 169 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 240 / 255.f, 183 / 255.f, 122 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 236 / 255.f, 159 / 255.f, 74 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 234 / 255.f, 146 / 255.f, 37 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 193 / 255.f, 111 / 255.f, 28 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 152 / 255.f, 89 / 255.f, 22 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 110 / 255.f, 64 / 255.f, 16 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 80 / 255.f, 47 / 255.f, 12 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 82 / 255.f, 82 / 255.f, 82 / 255.f, 1.0f });
			}
			//8
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 247 / 255.f, 218 / 255.f, 170 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 244 / 255.f, 200 / 255.f, 124 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 241 / 255.f, 182 / 255.f, 77 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 239 / 255.f, 174 / 255.f, 44 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 196 / 255.f, 137 / 255.f, 34 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 154 / 255.f, 108 / 255.f, 27 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 111 / 255.f, 77 / 255.f, 19 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 80 / 255.f, 56 / 255.f, 14 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 54 / 255.f, 54 / 255.f, 54 / 255.f, 1.0f });
			}
			//9
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 254 / 255.f, 243 / 255.f, 187 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 253 / 255.f, 237 / 255.f, 153 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 253 / 255.f, 231 / 255.f, 117 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 254 / 255.f, 232 / 255.f, 85 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 242 / 255.f, 212 / 255.f, 53 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 192 / 255.f, 169 / 255.f, 42 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 138 / 255.f, 120 / 255.f, 30 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 101 / 255.f, 87 / 255.f, 22 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 29 / 255.f, 29 / 255.f, 29 / 255.f, 1.0f });
			}
			//10
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 247 / 255.f, 243 / 255.f, 185 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 243 / 255.f, 239 / 255.f, 148 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 239 / 255.f, 232 / 255.f, 111 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 235 / 255.f, 229 / 255.f, 76 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 208 / 255.f, 200 / 255.f, 55 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 164 / 255.f, 157 / 255.f, 43 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 118 / 255.f, 114 / 255.f, 31 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 86 / 255.f, 82 / 255.f, 21 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 9 / 255.f, 9 / 255.f, 9 / 255.f, 1.0f });
			}
			//11
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 218 / 255.f, 232 / 255.f, 182 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 198 / 255.f, 221 / 255.f, 143 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 181 / 255.f, 210 / 255.f, 103 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 154 / 255.f, 186 / 255.f, 76 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 130 / 255.f, 155 / 255.f, 64 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 102 / 255.f, 121 / 255.f, 50 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 74 / 255.f, 88 / 255.f, 36 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 54 / 255.f, 64 / 255.f, 26 / 255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 0 / 255.f, 0 / 255.f, 0 / 255.f, 1.0f });
			}



			if (isHoveredPicker)
			{
				if (Input::IsMouseClicked(0, elements_count, false))
				{

				}
			}
			else
			{
				if (Input::IsMouseClicked(0, elements_count, false))
				{
					active_picker = -1;
					//hover_element = false;
				}
			}
		}
		else
		{
			//Bg
			if (isHovered)
			{
				drawFilledRect(FVector2D{ pos.X, pos.Y }, size, size, Colors::Checkbox_Hovered);
				hover_element = true;
			}
			else
			{
				drawFilledRect(FVector2D{ pos.X, pos.Y }, size, size, Colors::Checkbox_Idle);
			}

			//Color
			drawFilledRect(FVector2D{ pos.X + 4, pos.Y + 4 }, size - 8, size - 8, *color);

			//Text
			FVector2D textPos = FVector2D{ pos.X + size + 5.0f, pos.Y - 5 };
			TextLeft(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			if (isHovered && Input::IsMouseClicked(0, elements_count, false))
			{
				saved_color = *color;
				active_picker = elements_count;
			}
		}


		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}


	void Render()
	{
		for (int i = 0; i < 128; i++)
		{
			if (PostRenderer::drawlist[i].type != -1)
			{
				//Filled Rect
				if (PostRenderer::drawlist[i].type == 1)
				{
					ZeroGUI::drawFilledRect(PostRenderer::drawlist[i].pos, PostRenderer::drawlist[i].size.X, PostRenderer::drawlist[i].size.Y, PostRenderer::drawlist[i].color);
				}
				//TextLeft
				else if (PostRenderer::drawlist[i].type == 2)
				{
					ZeroGUI::TextLeft(PostRenderer::drawlist[i].name, PostRenderer::drawlist[i].pos, PostRenderer::drawlist[i].color, PostRenderer::drawlist[i].outline);
				}
				//TextCenter
				else if (PostRenderer::drawlist[i].type == 3)
				{
					ZeroGUI::TextCenter(PostRenderer::drawlist[i].name, PostRenderer::drawlist[i].pos, PostRenderer::drawlist[i].color, PostRenderer::drawlist[i].outline);
				}
				//Draw_Line
				else if (PostRenderer::drawlist[i].type == 4)
				{
					Draw_Line(PostRenderer::drawlist[i].from, PostRenderer::drawlist[i].to, PostRenderer::drawlist[i].thickness, PostRenderer::drawlist[i].color);
				}

				PostRenderer::drawlist[i].type = -1;
			}
		}
	}
}

namespace Render
{
	bool ShowMenu = false;
	bool DrawCursor = true;
	UCanvas* Canvas;

	void line(FVector2D a, FVector2D b, FLinearColor render_color, float thickness) {
		Canvas->K2_DrawLine(a, b, thickness, render_color);
	}
	void circle(FVector2D position, int radius, int segments, FLinearColor color) {
		float PI = 3.1415927f;

		float Step = PI * 2.0 / segments;
		int Count = 0;
		FVector2D V[128];
		for (float a = 0; a < PI * 2.0; a += Step) {
			float X1 = radius * StaticClasses::KismetMathLibrary->cos(a) + position.X;
			float Y1 = radius * StaticClasses::KismetMathLibrary->sin(a) + position.Y;
			float X2 = radius * StaticClasses::KismetMathLibrary->cos(a + Step) + position.X;
			float Y2 = radius * StaticClasses::KismetMathLibrary->sin(a + Step) + position.Y;
			V[Count].X = X1;
			V[Count].Y = Y1;
			V[Count + 1].X = X2;
			V[Count + 1].Y = Y2;
			line(FVector2D{ V[Count].X, V[Count].Y }, FVector2D{ X2, Y2 }, color, 1.f);
		}
	}

	void filled_box(FVector2D position, FVector2D size, FLinearColor render_color) {
		for (int i = 0; i < size.Y; i++) {
			Canvas->K2_DrawLine(FVector2D(position.X, position.Y + i), FVector2D(position.X + size.X, position.Y + i), 1.f, render_color);
		}
	}
}

namespace Menu
{
	FVector2D Pos = { 25, 25 };

	static void Render(UCanvas* Canvas)
	{
		if (Render::ShowMenu)
		{
			ZeroGUI::SetupCanvas(Canvas);
			ZeroGUI::Input::Handle();

			float Width = ((double)Canvas->ClipX());
			float Height = ((double)Canvas->ClipY());
			if (ZeroGUI::Window("Cold.cc Standard Edition", &Pos, FVector2D{ 500.0f, 650.0f }, Render::ShowMenu, Width, Height))
			{
				static int Tab = 0;

				if (ZeroGUI::ButtonTab(("Aimbot"), FVector2D{ 118.5, 25 }, Tab == 0)) Tab = 0;
				ZeroGUI::SameLine();
				if (ZeroGUI::ButtonTab(("Visuals"), FVector2D{ 118.5, 25 }, Tab == 0)) Tab = 1;
				ZeroGUI::SameLine();
				if (ZeroGUI::ButtonTab(("World"), FVector2D{ 118.5, 25 }, Tab == 0)) Tab = 2;
				ZeroGUI::SameLine();
				if (ZeroGUI::ButtonTab(("Misc"), FVector2D{ 118.5, 25 }, Tab == 0)) Tab = 3;



				if (Tab == 0)
				{
					ZeroGUI::Checkbox("Enable", &Settings::Combat::Enable);
					ZeroGUI::Checkbox("Visible Check", &Settings::Misc::VisibleCheck);
					ZeroGUI::Checkbox("Prediction", &Settings::Combat::Prediction);
					ZeroGUI::Checkbox("Triggerbot", &Settings::Combat::Triggerbot);
					ZeroGUI::Checkbox("Ignore Knocked", &Settings::Misc::SkipKnocked);
					ZeroGUI::Checkbox("Ignore Bots", &Settings::Misc::SkipBot);
					ZeroGUI::Checkbox("Aim Line", &Settings::Combat::MuzzleLine);
					ZeroGUI::Checkbox("Show FOV", &Settings::Combat::DrawFOV);
					ZeroGUI::SliderFloat("Smoothing", &Settings::Combat::Smooth, 1, 25);
					ZeroGUI::SliderFloat("FOV Value", &Settings::Combat::AimFOV, 1, 360);

					FVector2D Size = FVector2D{ 150, 25 };
					ZeroGUI::Text("Aimbot Key");
					ZeroGUI::Hotkey("", Size, &Settings::Combat::AimbotKeybind);
					ZeroGUI::Text("Trigger Key");
					ZeroGUI::Hotkey2("", Size, &Settings::Combat::TriggerKeybind);
				}

				else if (Tab == 1)
				{
					ZeroGUI::Checkbox("Box", &Settings::Visuals::Box);
					ZeroGUI::Checkbox("Corner Box", &Settings::Visuals::CornerBox);
					ZeroGUI::Checkbox("Filled Box", &Settings::Visuals::Filledbox);
					ZeroGUI::Checkbox("Skeleton", &Settings::Visuals::Skeleton);
					ZeroGUI::Checkbox("Name", &Settings::Visuals::Name);
					ZeroGUI::Checkbox("Platform", &Settings::Visuals::Platform);
					ZeroGUI::Checkbox("Distance", &Settings::Visuals::Distance);
					ZeroGUI::Checkbox("Weapon", &Settings::Visuals::Weapon);
					ZeroGUI::Checkbox("Team ID", &Settings::Visuals::TeamID);
					ZeroGUI::Text("");
					ZeroGUI::SliderFloat("Skeleton Thickness", &Settings::Visuals::SkeletonThickness, 1, 5);
					ZeroGUI::SliderFloat("Box Thickness", &Settings::Visuals::BoxThickness, 1, 5);
				}
				else if (Tab == 2)
				{
					ZeroGUI::Checkbox("Enable World", &Settings::World::Enable);
					ZeroGUI::Checkbox("Pickup", &Settings::World::Pickup);

					ZeroGUI::SliderFloat("Loot Distance", &Settings::World::MaxDistance, 1, 300);
				}

				else if (Tab == 3)
				{
					ZeroGUI::Checkbox("Ignore Knocked", &Settings::Misc::IgnoreKnocked);
					ZeroGUI::Checkbox("Ignore Dying", &Settings::Misc::IgnoreDying);
					ZeroGUI::Checkbox("Ignore Bots", &Settings::Misc::IgnoreBots);
				}
			}
		}
	}
}