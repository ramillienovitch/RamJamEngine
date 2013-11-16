#include "DX11SpriteBatch.h"

//////////////////////////////////////////////////////////////////////////
DX11SpriteBatch::DX11SpriteBatch()
{
	mVertexBuffer = nullptr;
	mIndexBuffer  = nullptr;
	mInputLayout  = nullptr;
	mBatchTexSRV  = nullptr;

	mInitialized  = false;
	mScreenWidth  = 0.0f;
	mScreenHeight = 0.0f;
	mTexWidth  = 0;
	mTexHeight = 0;
}

//////////////////////////////////////////////////////////////////////////
DX11SpriteBatch::~DX11SpriteBatch()
{
	RJE_SAFE_RELEASE(mVertexBuffer);
	RJE_SAFE_RELEASE(mIndexBuffer);
	RJE_SAFE_RELEASE(mInputLayout);
}

//////////////////////////////////////////////////////////////////////////
HRESULT DX11SpriteBatch::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	// Prevent double Init.
	RJE_ASSERT(!mInitialized);

	HRESULT hr = S_OK;

	mContext = context;

	mSpriteList.reserve(128);

	vector<USHORT> indices(BatchSize*6);
	for(UINT i = 0; i < BatchSize; ++i)
	{
		indices[i*6+0] = i*4+0;
		indices[i*6+1] = i*4+1;
		indices[i*6+2] = i*4+2;

		indices[i*6+3] = i*4+0;
		indices[i*6+4] = i*4+2;
		indices[i*6+5] = i*4+3;
	}

	D3D11_SUBRESOURCE_DATA indexData = {0};
	indexData.pSysMem = &indices[0];

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_B8G8R8A8_UNORM,  0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};	

	D3DX11_PASS_DESC passDesc;
	DX11Effects::SpriteFX->SpriteTech->GetPassByIndex(0)->GetDesc(&passDesc);
	hr = device->CreateInputLayout(layoutDesc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout);
	if(FAILED(hr))
		return hr;

	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth           = BatchSize*4*sizeof(MeshData::SpriteVertex);
	vbd.Usage               = D3D11_USAGE_DYNAMIC;
	vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags           = 0;
	vbd.StructureByteStride = 0;

	hr = device->CreateBuffer(&vbd, 0, &mVertexBuffer);
	if(FAILED(hr))
		return hr;

	// Index buffer is immutable.
	D3D11_BUFFER_DESC ibd;
	ibd.ByteWidth           = BatchSize*6*sizeof(USHORT);
	ibd.Usage               = D3D11_USAGE_IMMUTABLE;
	ibd.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags      = 0;
	ibd.MiscFlags           = 0;
	ibd.StructureByteStride = 0;

	hr = device->CreateBuffer(&ibd, &indexData, &mIndexBuffer);
	if(FAILED(hr))
		return hr;

	mInitialized = true;

	return hr;
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::BeginBatch(ID3D11ShaderResourceView* texSRV)
{
	RJE_ASSERT(mInitialized);

	mBatchTexSRV = texSRV;
	mBatchTexSRV->AddRef();

	// Get the size of the texture
	ID3D11Resource* resource = nullptr;
	mBatchTexSRV->GetResource(&resource);
	ID3D11Texture2D* tex = reinterpret_cast<ID3D11Texture2D*>(resource);

	D3D11_TEXTURE2D_DESC texDesc;
	tex->GetDesc(&texDesc);

	RJE_SAFE_RELEASE(resource);

	mTexWidth  = texDesc.Width; 
	mTexHeight = texDesc.Height;

	mSpriteList.clear();
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::EndBatch(ID3D11DeviceContext* dc)
{
	assert(mInitialized);

	UINT viewportCount = 1;
	D3D11_VIEWPORT vp;
	dc->RSGetViewports(&viewportCount, &vp);

	mScreenWidth  = vp.Width;
	mScreenHeight = vp.Height;

	UINT stride = sizeof(MeshData::SpriteVertex);
	UINT offset = 0;
	dc->IASetInputLayout(mInputLayout);
	dc->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	dc->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DX11Effects::SpriteFX->SetSpriteMap(mBatchTexSRV);

	ID3DX11EffectPass* pass = DX11Effects::SpriteFX->SpriteTech->GetPassByIndex(0);
	pass->Apply(0, dc);

	UINT spritesToDraw = (UINT)mSpriteList.size();
	UINT startIndex = 0;

	while( spritesToDraw > 0 )
	{
		if( spritesToDraw <= BatchSize )
		{
			DrawBatch(dc, startIndex, spritesToDraw);
			spritesToDraw = 0;
		}
		else
		{
			DrawBatch(dc, startIndex, BatchSize);
			startIndex += BatchSize;
			spritesToDraw -= BatchSize;
		}
	}

	RJE_SAFE_RELEASE(mBatchTexSRV);
}

#pragma region Draw Functions
//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::Draw(const POINT& position, XMCOLOR color)
{
	Sprite sprite;
	sprite.SrcRect  = CD3D11_RECT(0, 0, mTexWidth, mTexHeight);
	sprite.DestRect = CD3D11_RECT(position.x, position.y, position.x + mTexWidth, position.y + mTexHeight);
	sprite.Color    = color;

	mSpriteList.push_back(sprite);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::Draw(const POINT& position, const CD3D11_RECT& sourceRect, XMCOLOR color)
{
	int srcWidth  = sourceRect.right - sourceRect.left;
	int srcHeight = sourceRect.bottom - sourceRect.top;

	Sprite sprite;
	sprite.SrcRect  = sourceRect;
	sprite.DestRect = CD3D11_RECT(position.x, position.y, position.x + srcWidth, position.y + srcHeight);
	sprite.Color    = color;

	mSpriteList.push_back(sprite);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::Draw(const POINT& center, float width, float height, XMCOLOR color)
{
	Sprite sprite;
	sprite.SrcRect  = CD3D11_RECT(0, 0, mTexWidth, mTexHeight);;
	sprite.DestRect = CD3D11_RECT(center.x - (LONG)(0.5*width), center.y - (LONG)(0.5*width), center.x + (LONG)(0.5*width), center.y + (LONG)(0.5*height));
	sprite.Color    = color;

	mSpriteList.push_back(sprite);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::Draw(const POINT& position, const CD3D11_RECT& sourceRect, XMCOLOR color, float z, float angle, float scale)
{
	int srcWidth  = sourceRect.right - sourceRect.left;
	int srcHeight = sourceRect.bottom - sourceRect.top;

	Sprite sprite;
	sprite.SrcRect = sourceRect;
	sprite.DestRect = CD3D11_RECT(position.x, position.y, position.x + srcWidth, position.y + srcHeight);
	sprite.Color  = color;
	sprite.Z      = z;
	sprite.Angle  = angle;
	sprite.Scale  = scale;

	mSpriteList.push_back(sprite);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::Draw(const CD3D11_RECT& destinationRect, XMCOLOR color)
{
	Sprite sprite;
	sprite.SrcRect  = CD3D11_RECT(0, 0, mTexWidth, mTexHeight);
	sprite.DestRect = destinationRect;
	sprite.Color    = color;

	mSpriteList.push_back(sprite);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::Draw(const CD3D11_RECT& destinationRect, const CD3D11_RECT& sourceRect, XMCOLOR color)
{
	Sprite sprite;
	sprite.SrcRect  = sourceRect;
	sprite.DestRect = destinationRect;
	sprite.Color    = color;

	mSpriteList.push_back(sprite);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::Draw(const CD3D11_RECT& destinationRect, const CD3D11_RECT& sourceRect, XMCOLOR color, float z, float angle, float scale)
{
	Sprite sprite;
	sprite.SrcRect  = sourceRect;
	sprite.DestRect = destinationRect;
	sprite.Color    = color;
	sprite.Z        = z;
	sprite.Angle    = angle;
	sprite.Scale    = scale;

	mSpriteList.push_back(sprite);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::DrawTexture2D(ID3D11ShaderResourceView* texSRV, ID3D11DeviceContext* dc, CD3D11_RECT rect, XMCOLOR color)
{
	BeginBatch(texSRV);
	Draw(rect, color);
	EndBatch(dc);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::DrawString(DX11FontSheet& fs, const std::wstring& text, const POINT& pos, XMCOLOR color, TextAlignment alignment)
{
	BeginBatch(fs.GetFontSheetSRV());

	UINT length = (UINT)text.length();

	int posX       = 0;
	int posY       = pos.y;
	int totalWidth = 0;
	
	if (alignment == LeftAligned)
	{
		posX = pos.x;
	}
	else
	{
		// Calculate text total width to adjust with the alignment
		for(UINT i = 0; i < length; ++i)
		{
			WCHAR character = text[i];
		
			if(character == ' ')
			{
				totalWidth += fs.GetMaxCharWidth();
			}
			else
			{
				if ( character < DX11FontSheet::StartChar || character > DX11FontSheet::EndChar )
					character = '?';

				// Get the bounding rect of the character on the fontsheet.
				const CD3D11_RECT& charRect = fs.GetCharBoundingRect(character);

				int width  = charRect.right - charRect.left;
				totalWidth += width+1;
			}
		}

		if (alignment == RightAligned)		posX = pos.x - totalWidth;
		else								posX = pos.x - totalWidth/ 2;
	}

	for(UINT i = 0; i < length; ++i)
	{
		WCHAR character = text[i];

		if(character == ' ')
		{
			posX += fs.GetMaxCharWidth();
		}
		else if(character == '\n')
		{
			if (alignment == LeftAligned)
				posX = pos.x;
			posY += fs.GetCharHeight();
		}
		else
		{
			if ( character < DX11FontSheet::StartChar || character > DX11FontSheet::EndChar )
				character = '?';

			// Get the bounding rect of the character on the fontsheet.
			const CD3D11_RECT& charRect = fs.GetCharBoundingRect(character);

			int width  = charRect.right - charRect.left;
			int height = charRect.bottom - charRect.top;

			// Draw the character sprite.
			Draw(CD3D11_RECT(posX, posY, posX + width, posY + height), charRect, color);

			// Move to the next character position.
			posX += width + 1;
		}
	}

	EndBatch(mContext);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::DrawString(DX11FontSheet& fs, const char text[], const POINT& pos, XMCOLOR color, TextAlignment alignment)
{
	BeginBatch(fs.GetFontSheetSRV());

	UINT length = (UINT)strlen(text);

	int posX       = 0;
	int posY       = pos.y;
	int totalWidth = 0;

	if (alignment == LeftAligned)
	{
		posX = pos.x;
	}
	else
	{
		// Calculate text total width to adjust with the alignment
		for(UINT i = 0; i < length; ++i)
		{
			WCHAR character = text[i];

			if(character == ' ')
			{
				totalWidth += fs.GetMaxCharWidth();
			}
			else
			{
				if ( character < DX11FontSheet::StartChar || character > DX11FontSheet::EndChar )
					character = '?';

				// Get the bounding rect of the character on the fontsheet.
				const CD3D11_RECT& charRect = fs.GetCharBoundingRect(character);

				int width  = charRect.right - charRect.left;
				totalWidth += width+1;
			}
		}

		if (alignment == RightAligned)		posX = pos.x - totalWidth;
		else								posX = pos.x - totalWidth/ 2;
	}

	for(UINT i = 0; i < length; ++i)
	{
		WCHAR character = text[i];

		if(character == ' ') 
		{
			posX += fs.GetMaxCharWidth();
		}
		else if(character == '\n')
		{
			posX  = pos.x;
			posY += fs.GetCharHeight();
		}
		else
		{
			if ( character < DX11FontSheet::StartChar || character > DX11FontSheet::EndChar )
				character = '?';

			// Get the bounding rect of the character on the fontsheet.
			const CD3D11_RECT& charRect = fs.GetCharBoundingRect(character);

			int width  = charRect.right - charRect.left;
			int height = charRect.bottom - charRect.top;

			// Draw the character sprite.
			Draw(CD3D11_RECT(posX, posY, posX + width, posY + height), charRect, color);

			// Move to the next character position.
			posX += width + 1;
		}
	}

	EndBatch(mContext);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::DrawInfoText(DX11FontSheet& fs, const char* text, const POINT& pos)
{
	BeginBatch(fs.GetFontSheetSRV());

	int posX = pos.x;
	int posY = pos.y;

	XMCOLOR color = Color::Black;

	UINT i = 0;
	while( true )
	{
		WCHAR character = text[i];

		if(character == ' ') 
			posX += fs.GetSpaceWidth();
		else if(character == '\n')
		{
			posX  = pos.x;
			posY += fs.GetCharHeight();
		}
		else if(character == SCREEN_WHITE)		color = Color::White;
		else if(character == SCREEN_RED)		color = Color::Red;
		else if(character == SCREEN_GREEN)		color = Color::Lime;
		else if(character == SCREEN_BLUE)		color = Color::Blue;
		else if(character == SCREEN_YELLOW)		color = Color::Yellow;
		else if(character == SCREEN_ORANGE)		color = Color::Orange;
		else if(character == SCREEN_ROSE)		color = Color::Fuchsia;
		else if(character == SCREEN_GRAY)		color = Color::Gray;
		else if(character == SCREEN_BLACK)		color = Color::Black;
		else if(character == nullchar)			break;
		else
		{
			if ( character < DX11FontSheet::StartChar || character > DX11FontSheet::EndChar )
				character = '?';

			// Get the bounding rect of the character on the fontsheet.
			const CD3D11_RECT& charRect = fs.GetCharBoundingRect(character);

			int width  = charRect.right - charRect.left;
			int height = charRect.bottom - charRect.top;

			// Draw the character sprite.
			Draw(CD3D11_RECT(posX, posY, posX + width, posY + height), charRect, color);

			// Move to the next character position.
			posX += width + 1;
		}
		++i;
	}

	EndBatch(mContext);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::DrawConsoleCommand(DX11FontSheet& fs, char (&text)[COMMAND_MAX_LENGTH], const POINT& pos)
{
	BeginBatch(fs.GetFontSheetSRV());

	int posX = pos.x;
	int posY = pos.y;

	XMCOLOR color = 0xffffffff;
	
	for(UINT i = 0; i < COMMAND_MAX_LENGTH; ++i)
	{
		WCHAR character = text[i];

		if(character == ' ')
			posX += fs.GetSpaceWidth();
		else if(character == '\n')
		{
			posX  = pos.x;
			posY += fs.GetCharHeight();
		}
		else if(character == nullchar)
			break;
		else
		{
			if ( character < DX11FontSheet::StartChar || character > DX11FontSheet::EndChar )
				character = '?';

			const CD3D11_RECT& charRect = fs.GetCharBoundingRect(character);

			int width  = charRect.right - charRect.left;
			int height = charRect.bottom - charRect.top;

			Draw(CD3D11_RECT(posX, posY, posX + width, posY + height), charRect, color);

			// Move to the next character position.
			posX += width + 1;
		}
	}

	// We Draw the cursor
	const CD3D11_RECT& charRect = fs.GetCharBoundingRect('_');
	int width  = charRect.right - charRect.left;
	int height = charRect.bottom - charRect.top;

	Draw(CD3D11_RECT(posX, posY, posX + width, posY + height), charRect, color);

	EndBatch(mContext);
}

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::DrawBatch(ID3D11DeviceContext* dc, UINT startSpriteIndex, UINT spriteCount)
{
	// Write the quads to the vertex buffer.
	D3D11_MAPPED_SUBRESOURCE mappedData;
	dc->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	MeshData::SpriteVertex* v = reinterpret_cast<MeshData::SpriteVertex*>(mappedData.pData);

	for(UINT i = 0; i < spriteCount; ++i)
	{
		const Sprite& sprite = mSpriteList[startSpriteIndex + i];

		MeshData::SpriteVertex quad[4];
		BuildSpriteQuad(sprite, quad);

		v[i*4+0] = quad[0];
		v[i*4+1] = quad[1];
		v[i*4+2] = quad[2];
		v[i*4+3] = quad[3];
	}

	dc->Unmap(mVertexBuffer, 0);

	dc->DrawIndexed(spriteCount*6, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
Vector3 DX11SpriteBatch::PointToNdc(int x, int y, float z)
{ return Vector3(2.0f*(float)x/mScreenWidth - 1.0f, 1.0f - 2.0f*(float)y/mScreenHeight, z); }

//////////////////////////////////////////////////////////////////////////
void DX11SpriteBatch::BuildSpriteQuad(const Sprite& sprite, MeshData::SpriteVertex v[4])
{
	const CD3D11_RECT& dest = sprite.DestRect;
	const CD3D11_RECT& src  = sprite.SrcRect;

	// Dest rect defines target in screen space.
	v[0].pos = PointToNdc(dest.left,  dest.bottom, sprite.Z);
	v[1].pos = PointToNdc(dest.left,  dest.top,    sprite.Z);
	v[2].pos = PointToNdc(dest.right, dest.top,    sprite.Z);
	v[3].pos = PointToNdc(dest.right, dest.bottom, sprite.Z);

	// Source rect defines subset of texture to use from sprite sheet.
	v[0].tex = Vector2((float)src.left  / mTexWidth, (float)src.bottom / mTexHeight); 
	v[1].tex = Vector2((float)src.left  / mTexWidth, (float)src.top    / mTexHeight); 
	v[2].tex = Vector2((float)src.right / mTexWidth, (float)src.top    / mTexHeight); 
	v[3].tex = Vector2((float)src.right / mTexWidth, (float)src.bottom / mTexHeight); 

	v[0].color = (ARGB)sprite.Color;
	v[1].color = (ARGB)sprite.Color;
	v[2].color = (ARGB)sprite.Color;
	v[3].color = (ARGB)sprite.Color;

	// Quad center point.
	float tx = 0.5f*(v[0].pos.x + v[3].pos.x);
	float ty = 0.5f*(v[0].pos.y + v[1].pos.y);

	XMVECTOR scaling     = XMVectorSet(sprite.Scale, sprite.Scale, 1.0f, 0.0f);
	XMVECTOR origin      = XMVectorSet(tx, ty, 0.0f, 0.0f);
	XMVECTOR translation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMMATRIX T           = XMMatrixAffineTransformation2D(scaling, origin, sprite.Angle, translation);

	// Rotate and scale the quad in NDC space.
	for(int i = 0; i < 4; ++i)
	{
		XMFLOAT3 vec = v[i].pos;
		XMVECTOR p = XMLoadFloat3(&vec);
		p = XMVector3TransformCoord(p, T);
		XMStoreFloat3(&vec, p);
		v[i].pos.Set(vec.x, vec.y, vec.z);
	}
}
