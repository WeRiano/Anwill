#pragma once

#include <unordered_map>
#include <queue>
#include <set>

#include "core/Timestamp.h"
#include "gfx/SpriteSheet.h"

#undef max

namespace Anwill {

    /**
     * Animation of frames.
     * Defined by the time between frames (frameDelta) and a bunch of frames (sprites).
     */
    class SpriteAnimation {
    public:
        SpriteAnimation(const Timestamp& frameDelta);

        void Tick(const Timestamp& delta);
        void AddFrame(const std::shared_ptr<Texture>& texture,
                      const QuadTexCoords& texCoords);
        void AddFrame(const std::shared_ptr<SpriteSheet>& spriteSheet,
                      unsigned int spriteSheetXPos, unsigned int spriteSheetYPos,
                      int pixelLeftPad = 0, int pixelRightPad = 0,
                      int pixelBottomPad = 0, int pixelTopPad = 0);
        /**
         * @brief
         * @param spriteSheet
         * @param startXPos
         * @param startYPos
         * @param increment positive = right
         * @param count
         */
        void AddFramesHorizontally(const std::shared_ptr<SpriteSheet>& spriteSheet,
                                   unsigned int startXPos, unsigned int startYPos,
                                   unsigned int increment, unsigned int count);
        /**
         * @brief
         * @param spriteSheet
         * @param startXPos
         * @param startYPos
         * @param increment positive = up
         * @param count
         */
        void AddFramesVertically(const std::shared_ptr<SpriteSheet>& spriteSheet,
                                   unsigned int startXPos, unsigned int startYPos,
                                   unsigned int increment, unsigned int count);
        /**
         * @brief
         * @param spriteSheet
         * @param startXPos
         * @param startYPos
         * @param increment positive = up and right
         * @param count
         */
        void AddFramesDiagonally(const std::shared_ptr<SpriteSheet>& spriteSheet,
                                 unsigned int startXPos, unsigned int startYPos,
                                 unsigned int increment, unsigned int count);
        void IncrementFrameDelta(const Timestamp& delta,
                                 const Timestamp& min = Timestamp(0),
                                 const Timestamp& max
                                 = Timestamp(std::numeric_limits<long long>::max()));
        void IncrementFrameDelta(double f, const Timestamp& max
                                 = Timestamp(std::numeric_limits<long long>::max()));
        Sprite GetActiveFrame() const;
        void SetFrameDelta(const Timestamp& frameDelta);

    private:
        // TODO: Why is this used instead of Sprite??
        struct SpriteFrame {
            unsigned int id; // TODO: ... why store an iD that is linked to a texture?? Just add the texture directly
            QuadTexCoords texCoords;
        };

        Timestamp m_FrameDelta;
        Timestamp m_Current;
        // TODO: Just have a stack of texture pointers ... and just use regular texCoords everywhere for now?
        std::queue<SpriteFrame> m_FrameQ;
        // I'm not sure if a combination of these two are the best solution to the problem.
        // A lot better than storing the same texture for every frame if all frames
        // come from 1 spritesheet (which is the intended way AND worst case)
        std::unordered_map<unsigned int, std::shared_ptr<Texture>> m_IDToTex;
        std::unordered_map<std::shared_ptr<Texture>, unsigned int> m_TexToID;

    };
}