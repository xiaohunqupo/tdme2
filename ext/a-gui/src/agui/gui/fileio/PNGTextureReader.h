#pragma once

#include <map>
#include <string>
#include <vector>

#include <agui/agui.h>
#include <agui/gui/fileio/fwd-agui.h>
#include <agui/gui/textures/fwd-agui.h>

#include <agui/utilities/ByteBuffer.h>

#include <ext/libpng/png.h>

using std::map;
using std::string;
using std::vector;

// namespaces
namespace agui {
namespace gui {
namespace fileio {
	using ::agui::gui::textures::GUITexture;
	using ::agui::utilities::ByteBuffer;
}
}
}

/**
 * PNG texture reader class
 * @author Andreas Drewke
 */
class agui::gui::fileio::PNGTextureReader final
{
	friend class agui::gui::textures::GUITexture;

public:
	/**
	 * Read PNG header from memory
	 * @param pngData png vector data to read PNG from
	 * @param width width
	 * @param height height
	 * @param bytes per pixel
	 * @returns success
	 */
	static bool readHeader(const vector<uint8_t>& pngData, int& width, int& height, uint8_t& bytesPerPixel);

	/**
	 * Read PNG from memory into texture byte buffer
	 * @param pngData png vector data to read PNG from
	 * @param textureByteBuffer texture byte buffer
	 * @returns success
	 */
	static bool read(const vector<uint8_t>& pngData, ByteBuffer& textureByteBuffer);

	/**
	 * Read PNG from memory
	 * @param textureId texture id
	 * @param pngData png vector data to read PNG from
	 * @param powerOfTwo scale image to fit power of two dimensions
	 * @param idPrefix id prefix
	 */
	static GUITexture* read(const string& textureId, const vector<uint8_t>& pngData, bool powerOfTwo = true, const string& idPrefix = string());

private:
	/**
	 * PNG input stream
	 */
	class PNGInputStream {
	public:

		/**
		 * Public constructor
		 * @param data data
		 */
		PNGInputStream(const vector<uint8_t>* data): offset(0), data(data) {
		}

		/**
		 * Destructor
		 */
		~PNGInputStream() {
		}

		/**
		 * Read bytes
		 * @param outBytes out bytes
		 * @param outBytesToRead out bytes to read
		 */
		void readBytes(int8_t* outBytes, int32_t outBytesToRead) {
			for (int32_t i = 0; i < outBytesToRead && offset < data->size(); i++) {
				outBytes[i] = (*data)[offset++];
			}
		}

	private:
		int offset;
		const vector<uint8_t>* data;

	};

	/**
	 * Read PNG data from memory
	 * @param png_ptr png structure
	 * @param outBytes out bytes
	 * @param outBytesToRead out bytes to read
	 */
	static void readDataFromMemory(png_structp png_ptr, png_bytep outBytes, png_size_t outBytesToRead);

};