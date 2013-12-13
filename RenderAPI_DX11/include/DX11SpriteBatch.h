#pragma once

#include "DX11Helper.h"

using namespace DirectX::PackedVector;

class DX11FontSheet;

typedef enum TextAlignment
{
	LeftAligned = 0,
	RightAligned,
	Centered
}TextAlignment;

class DX11SpriteBatch
{
public:
	DX11SpriteBatch();
	~DX11SpriteBatch();

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

	/// Begins a new sprite batch.  All sprites in the batch will use the given texture.
	//  All Draw() calls add a sprite to the batch.
	void BeginBatch(ID3D11ShaderResourceView* texSRV);

	/// Draw the current sprite batch and empties the internal sprite batch list.
	void EndBatch(ID3D11DeviceContext* dc);

	/// Adds a sprite to the sprite batch.
	//  This call is undefined if not called within a BeginBatch()/EndBatch().
	void Draw(const POINT& position, XMCOLOR color);
	void Draw(const POINT& position, const CD3D11_RECT& sourceRect,	XMCOLOR color);
	void Draw(const POINT& center, float width, float height, XMCOLOR color);
	void Draw(const POINT& position, const CD3D11_RECT& sourceRect, XMCOLOR color, float z, float angle, float scale);
	void Draw(const CD3D11_RECT& destinationRect, XMCOLOR color);
	void Draw(const CD3D11_RECT& destinationRect, const CD3D11_RECT& sourceRect, XMCOLOR color);
	void Draw(const CD3D11_RECT& destinationRect, const CD3D11_RECT& sourceRect, XMCOLOR color, float z, float angle, float scale);

	/// Draws a texture to the screen.
	void DrawTexture2D(ID3D11ShaderResourceView* texSRV, ID3D11DeviceContext* dc, CD3D11_RECT rect,XMCOLOR color);

	/// Draws a string to the screen.
	//  DrawString should not be called inside BeginBatch()/EndBatch().
	//  Internally, DrawString calls BeginBatch()/EndBatch() to draw the string characters as a single batch.
	void DrawString(        DX11FontSheet& fs, const std::wstring& text,         const POINT& pos, XMCOLOR color, TextAlignment alignment = LeftAligned);
	void DrawString(        DX11FontSheet& fs, const char text[],                const POINT& pos, XMCOLOR color, TextAlignment alignment = LeftAligned);
	void DrawInfoText(      DX11FontSheet& fs, const char* text,                 const POINT& pos);
	void DrawConsoleCommand(DX11FontSheet& fs, char (&text)[COMMAND_MAX_LENGTH], const POINT& pos);

private:

	struct Sprite
	{
		Sprite()
		{
			Color = XMCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			Z     = 0.0f;
			Angle = 0.0f;
			Scale = 1.0f;
		}

		CD3D11_RECT SrcRect;
		CD3D11_RECT DestRect;
		XMCOLOR Color;
		float Z;
		float Angle;
		float Scale;
	};

	/// Helper method for drawing a subset of sprites in the batch.
	void DrawBatch(ID3D11DeviceContext* dc, u32 startSpriteIndex, u32 spriteCount);

	/// Convert screen space point to NDC space.
	Vector3 PointToNdc(int x, int y, float z);

	/// Generates quad for the given sprite. 
	void BuildSpriteQuad(const Sprite& sprite, MeshData::SpriteVertex v[4]);

private:
	static const int BatchSize = 512;

	bool mInitialized;

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	ID3D11DeviceContext* mContext;

	ID3D11InputLayout* mInputLayout;

	float mScreenWidth;
	float mScreenHeight;

	// Texture to use for current batch.
	ID3D11ShaderResourceView* mBatchTexSRV;

	u32 mTexWidth;
	u32 mTexHeight;

	// List of sprites to draw using the current batch texture.
	std::vector<Sprite> mSpriteList;
};