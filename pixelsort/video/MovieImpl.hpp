
/*
 *  MovieImpl.hpp
 *  sfeMovie project
 *
 *  Copyright (C) 2010-2014 Lucas Soltic
 *  lucas.soltic@orange.fr
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#ifndef SFEMOVIE_MOVIEIMPL_HPP
#define SFEMOVIE_MOVIEIMPL_HPP

#include <cstring>
#include <memory>
#include <string>
#include <stdexcept>
#include <SFML/Config.hpp>
#include "VideoStream.hpp"
#include "DebugTools/LayoutDebugger.hpp"

namespace sfe
{
    class Demuxer;
    
    class MovieImpl : public VideoStream::Delegate, public sf::Drawable
    {
    public:
        MovieImpl(sf::Transformable& movieView);
        virtual ~MovieImpl();
        
        /** Attemps to open a media file (movie or audio)
         *
         * Opening can fails either because of a wrong filename,
         * or because you tried to open a media file that has no supported
         * video or audio stream.
         *
         * @param filename the path to the media file
         * @return true on success, false otherwise
         */
        bool openFromFile(const std::string& filename);
        
        
        /** Return a description of all the streams of the given type contained in the opened media
         *
         * @param type the stream type (audio, video...) to return
         */
        const Streams& getStreams(MediaType type) const;
        
        /** Request activation of the given stream. In case another stream of the same kind is already active,
         * it is deactivated.
         *
         * @note When opening a new media file, the default behaviour is to automatically activate the first
         * found audio and video streams
         *
         * @warning This method can only be used when the movie is stopped
         *
         * @param streamDescriptor the descriptor of the stream to activate
         * @return true if the stream could be selected (ie. valid stream and movie is stopped)
         */
        bool selectStream(const StreamDescriptor& streamDescriptor);
        
        /** Start or resume playing the media playback
         *
         * This function starts the stream if it was stopped, resumes it if it was paused,
         * and restarts it from beginning if it was already playing. This function is non blocking
         * and lets the audio playback happen in the background. The video playback must be updated
         * with the update() method.
         */
        void play();
        
        
        /** Pauses the media playback
         *
         * If the media playback is already paused,
         * this does nothing, otherwise the playback is paused.
         */
        void pause();
        
        
        /** Stops the media playback. The playing offset is reset to the beginning.
         *
         * This function stops the stream if it was playing or paused, and does nothing
         * if it was already stopped. It also resets the playing position (unlike pause()).
         */
        void stop();
        
        
        /** Update the media status and eventually decode frames
         */
        void update();
        
        
        /** Sets the sound's volume (default is 100)
         *
         * @param volume the volume in range [0, 100]
         */
        void setVolume(float volume);
        
        
        /** Returns the current sound's volume
         *
         * @return the sound's volume, in range [0, 100]
         */
        float getVolume() const;
        
        
        /** Returns the duration of the movie
         *
         * @return the duration as sf::Time
         */
        sf::Time getDuration() const;
        
        
        /** Returns the size (width, height) of the currently active video stream
         *
         * @return the size of the currently active video stream, or (0, 0) is there is none
         */
        sf::Vector2f getSize() const;
        
        
        /** @see fitFrame(sf::FloatRect, bool)
         */
        void fit(float x, float y, float width, float height, bool preserveRatio = true);
        
        
        /** Scales the movie to fit the requested frame.
         *
         * If the ratio is preserved, the movie may be centered
         * in the given frame, thus the movie position may be different from
         * the one you specified.
         *
         * @note This method will erase any previously set scale and position
         *
         * @param frame the target frame in which you want to display the movie
         * @param preserveRatio true to keep the original movie ratio, false otherwise
         */
        void fit(sf::FloatRect frame, bool preserveRatio = true);
        
        
        /** Returns the average amount of video frames per second
         *
         * @return the average video frame rate
         */
        float getFramerate() const;
        
        
        /** Returns the amount of audio samples per second
         *
         * @return the audio sample rate
         */
        unsigned int getSampleRate() const;
        
        
        /** Returns the count of audio channels
         *
         * @return the channels' count
         */
        unsigned int getChannelCount() const;
        
        
        /** Returns the current status of the movie
         *
         * @return See enum Status
         */
        Status getStatus() const;
        
        
        /** Returns the current playing position in the movie
         *
         * @return the playing position
         */
        sf::Time getPlayingOffset() const;
        
        
        /** Returns the latest movie image
         *
         * The returned image is a texture in VRAM.
         * If the movie has no video stream, this returns an empty texture.
         *
         * @note As in the classic update()/draw() workflow, update() needs to be called
         * before using this method if you want the image to be up to date
         *
         * @return the current image of the movie for the activated video stream
         */
        const sf::Texture& getCurrentImage() const;
        
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void didUpdateVideo(const VideoStream& sender, const sf::Texture& image);
        
        float getVideoRotation() const;
        
    private:
        sf::Transformable& m_movieView;
        std::shared_ptr<Demuxer> m_demuxer;
        std::shared_ptr<Timer> m_timer;
        sf::Sprite m_videoSprite;
        Streams m_audioStreamsDesc;
        Streams m_videoStreamsDesc;
        sf::FloatRect m_displayFrame;
        LayoutDebugger<sf::Sprite> m_debugger;
    };
    
}

#endif
