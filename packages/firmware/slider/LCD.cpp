#include "inc/stdinc.h"

LCD::LCD()
    : m_LCD(D14, D15, D7)
    , m_MonochromeCanvas(128, 64)
{
}

void LCD::begin()
{
    // Configure control outputs for LCD
    analogWrite(LCDBrightnessPin, 255);

    m_LCD.begin(32 * 1000 * 1000);  // 32Mhz is the Particle Gen3 maximum
    m_LCD.setRotation(3);

    blitColorRegion(0, 0, m_LCD.width(), m_LCD.height(), RGBColor());
}

void LCD::blitColorRegion(
    uint16_t const x, uint16_t const y, uint16_t const width, uint16_t const height, RGBColor const color)
{
    // A (much) faster version of m_LCD.fillScreen
    std::array<uint16_t, DisplayWidth> pixelBuffer;  // about the maximum we're willing to pay in stack space
    pixelBuffer.fill(color.to565Color());

    m_LCD.startWrite();
    m_LCD.setAddrWindow(x, y, width, height);

    size_t cPixelsRemaining = width * height;

    while (cPixelsRemaining)
    {
        size_t const cPixelsToSend = std::min(cPixelsRemaining, pixelBuffer.size());
        SPI.transfer(pixelBuffer.data(), NULL, cPixelsToSend * sizeof(uint16_t), NULL);

        cPixelsRemaining -= cPixelsToSend;
    }

    m_LCD.endWrite();
}

void LCD::blitMonochromeCanvas(uint16_t const x,
                               uint16_t const y,
                               uint16_t const width,
                               uint16_t const height,
                               RGBColor const foregroundColor,
                               RGBColor const backgroundColor)
{
    std::array<uint16_t, DisplayWidth> pixelBuffer;  // ...about the maximum we're willing to pay in stack space

    m_LCD.startWrite();
    m_LCD.setAddrWindow(x, y, width, height);

    size_t cPixelsInBuffer = 0;

    uint16_t const foregroundColorValue = __bswap_16(foregroundColor.to565Color());
    uint16_t const backgroundColorValue = __bswap_16(backgroundColor.to565Color());

    for (uint16_t sourceY = 0; sourceY < height; ++sourceY)
    {
        for (uint16_t sourceX = 0; sourceX < width; ++sourceX)
        {
            pixelBuffer[cPixelsInBuffer] =
                m_MonochromeCanvas.getPixel(sourceX, sourceY) ? foregroundColorValue : backgroundColorValue;
            ++cPixelsInBuffer;

            if (cPixelsInBuffer == pixelBuffer.size())
            {
                SPI.transfer(pixelBuffer.data(), NULL, cPixelsInBuffer * sizeof(uint16_t), NULL);
                cPixelsInBuffer = 0;
            }
        }
    }

    if (cPixelsInBuffer)
    {
        SPI.transfer(pixelBuffer.data(), NULL, cPixelsInBuffer * sizeof(uint16_t), NULL);
    }

    m_LCD.endWrite();
}

void LCD::StaticText::setText(const char* const szText, ssize_t const idxCharacterToHighlight) const
{
    constexpr uint16_t c_AnyColor = static_cast<uint16_t>(-1);

    // Write to backbuffer
    GFXcanvas1& monochromeCanvas = m_Parent.m_MonochromeCanvas;
    {
        if (monochromeCanvas.width() < m_Rect.Width || monochromeCanvas.height() < m_Rect.Height)
        {
            // Consider tiling later
            return;
        }

        monochromeCanvas.fillScreen(0);

        monochromeCanvas.setTextColor(static_cast<uint16_t>(-1));
        monochromeCanvas.setFont(m_Font);
        monochromeCanvas.setTextSize(1);

        int16_t measuredX;
        int16_t measuredY;
        uint16_t measuredWidth;
        uint16_t measuredHeight;
        {
            monochromeCanvas.getTextBounds(szText, 0, 0, &measuredX, &measuredY, &measuredWidth, &measuredHeight);
        }

        switch (m_Alignment)
        {
            case Alignment::Left:
                monochromeCanvas.setCursor(0, -measuredY);
                break;

            case Alignment::Center:
                monochromeCanvas.setCursor((m_Rect.Width - measuredWidth) / 2, -measuredY);
                break;

            case Alignment::Right:
                monochromeCanvas.setCursor(m_Rect.Width - measuredWidth - 1 /* boundary pixel */ - 1 /* rounding error in AdaFruit library? */, -measuredY);
                break;
        }

        size_t const cchText = strlen(szText);

        for (size_t idxChar = 0; idxChar < cchText; ++idxChar)
        {
            int16_t const startCursorX = monochromeCanvas.getCursorX();

            monochromeCanvas.write(szText[idxChar]);

            bool const isCharacterToHighlight =
                idxCharacterToHighlight >= 0 && (cchText - idxChar - 1) == idxCharacterToHighlight;

            if (isCharacterToHighlight)
            {
                int16_t const endCursorX = monochromeCanvas.getCursorX();

                monochromeCanvas.fillRect(startCursorX,
                                          m_Rect.Height - m_CharacterHighlightHeight,
                                          endCursorX - startCursorX,
                                          m_CharacterHighlightHeight,
                                          c_AnyColor);
            }
        }
    }

    // Write backbuffer to device
    m_Parent.blitMonochromeCanvas(
        m_Rect.X, m_Rect.Y, m_Rect.Width, m_Rect.Height, m_ForegroundColor, m_BackgroundColor);
}

void LCD::StaticNumericText::setValue(int32_t const value)
{
    if (m_Value != value)
    {
        m_Value = value;
        update();
    }
}

void LCD::StaticNumericText::setActiveDigit(uint8_t const activeDigit)
{
    if (m_ActiveDigit != activeDigit)
    {
        m_ActiveDigit = activeDigit;
        update();
    }
}

void LCD::StaticNumericText::update()
{
    // Create text buffer
    char rgText[std::numeric_limits<decltype(m_Value)>::digits10 + 1 /* sign */ + 1 /* terminator */];
    {
        snprintf(rgText, countof(rgText), "%0*d", m_ActiveDigit + 1, m_Value);
    }

    setText(rgText, m_ActiveDigit);
}
