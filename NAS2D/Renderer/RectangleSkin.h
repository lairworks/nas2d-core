// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once


namespace NAS2D
{
	class Image;
	class Renderer;

	template <typename BaseType> struct Vector;
	template <typename BaseType> struct Rectangle;


	class RectangleSkin
	{
	public:
		RectangleSkin(const Image& topLeft, const Image& top, const Image& topRight, const Image& left, const Image& center, const Image& right, const Image& bottomLeft, const Image& bottom, const Image& bottomRight);

		Vector<int> minSize() const;

		void draw(Renderer& renderer, const Rectangle<int>& rect) const;

	private:
		const Image& mTopLeft;
		const Image& mTop;
		const Image& mTopRight;
		const Image& mLeft;
		const Image& mCenter;
		const Image& mRight;
		const Image& mBottomLeft;
		const Image& mBottom;
		const Image& mBottomRight;
	};
}
