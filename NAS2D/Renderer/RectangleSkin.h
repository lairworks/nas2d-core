#pragma once


namespace NAS2D
{
	class Image;
	class Renderer;
	template <typename BaseType> struct Rectangle;


	class RectangleSkin
	{
	public:
		RectangleSkin(const Image& topLeft, const Image& top, const Image& topRight, const Image& left, const Image& center, const Image& right, const Image& bottomLeft, const Image& bottom, const Image& bottomRight);

		void draw(Renderer& renderer, const Rectangle<float>& rect) const;

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
