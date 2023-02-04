#pragma once

struct IDirect3DTexture9;

struct texture_t {
	unsigned char pad0[ 0xC ];
	IDirect3DTexture9* m_raw_texture;
};