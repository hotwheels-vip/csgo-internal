#pragma once

namespace sdk
{
	struct i_texture {
	private:
		virtual void unk_0( ) = 0;
		virtual void unk_1( ) = 0;
		virtual void unk_2( ) = 0;

	public:
		virtual int get_width( ) const  = 0;
		virtual int get_height( ) const = 0;
	};
} // namespace sdk
