#pragma once

#include <unordered_map>
#include <queue>
#include <set>

#include "core/Timestamp.h"
#include "gfx/SpriteSheet.h"

#undef max

namespace Anwill {

    // TODO: This should be dependent on SPRITE!

    /**
     * Sprite animation.
     * Defined by a time between frames (frameDelta) and a bunch of frames (sprites).
     */
    class SpriteAnimation {
    public:
        SpriteAnimation(const Timestamp& frameDelta);

        /**
         * Progress the animation.
         * @param delta Amount of time in which the animation will progress.
         */
        void Tick(const Timestamp& delta);

        /**
         * Add a frame to the animation.
         * @param sprite New sprite frame.
         */
        void AddFrame(const Sprite& sprite);

        /**
         * Add a frame to the animation.
         * @param texture Frame texture.
         * @param texCoords Frame texture coordinates.
         */
        void AddFrame(const std::shared_ptr<Texture>& texture,
                      const QuadTexCoords& texCoords);

        /**
         * Add a frame to the animation from a spritesheet.
         * @param spriteSheet Spritesheet which the frame is from.
         * @param spriteSheetXPos Sprite X position on the sheet, starting from left.
         * @param spriteSheetYPos Sprite Y position on the sheet, starting from bottom.
         * @param leftPadPixels Number of pixels to add to the left of the sprite.
         * @param rightPadPixels Number of pixels to add to the right of the sprite.
         * @param bottomPadPixels Number of pixels to add to the top of the sprite.
         * @param topPadPixels Number of pixels to add to the bottom of the sprite.
         */
        void AddFrame(const std::shared_ptr<SpriteSheet>& spriteSheet,
                      unsigned int spriteSheetXPos, unsigned int spriteSheetYPos,
                      int leftPadPixels = 0, int rightPadPixels = 0,
                      int bottomPadPixels = 0, int topPadPixels = 0);

        /**
         * Add several frames to the animation from a spritesheet.
         * @param spriteSheet The Even @SpriteSheet to add frames from.
         * @param startXPos Sprite X position of the first sprite on the sheet.
         * @param startYPos Sprite Y position of the first sprite on the sheet.
         * @param xIncrement Horizontal increment. Positive values move to the right.
         * @param yIncrement Vertical increment. Positive values move upwards.
         * @param count Number of sprites to add.
         */
        void AddFrames(const std::shared_ptr<SpriteSheet>& spriteSheet,
                       unsigned int startXPos, unsigned int startYPos,
                       unsigned int xIncrement, unsigned int yIncrement,
                       unsigned int count);

        /**
         * Change the frame delta by incrementing it with a timestamp value.
         * @param delta Timestamp which the frame delta will be incremented with.
         * @param min Minimum resulting frame delta value.
         * @param max Maximum resulting frame delta value.
         */
        void ChangeFrameDelta(const Timestamp& delta,
                              const Timestamp& min = Timestamp(0),
                              const Timestamp& max
                                 = Timestamp(std::numeric_limits<long long>::max()));

        /*
        /**
         * Change the frame delta by incrementing or decrementing it.
         * @param delta Value to increment/decrement the frame delta with.
         * @param max Maximum resulting frame delta value.

        void ChangeFrameDelta(double delta);
        */

        /**
         * Check if the animation is empty (contains no frames).
         * @return True if the animation is empty and false otherwise.
         */
        bool IsEmpty() const;

        /**
         * Get the current frame of the animation.
         * Will crash if animation contains no frames.
         * @return The current frame.
         */
        Sprite GetActiveFrame() const;

        /**
         * Get the current time between frames.
         * @return Frame delta.
         */
        Timestamp GetFrameDelta() const;

        /**
         * Set the frame delta value.
         * @param frameDelta New frame delta.
         */
        void SetFrameDelta(const Timestamp& frameDelta);

    private:
        Timestamp m_FrameDelta;
        Timestamp m_Current;

        std::queue<Sprite> m_FrameQ;
    };
}