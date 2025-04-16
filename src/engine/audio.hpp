#include <SDL3_mixer/SDL_mixer.h>
#include <utility/log.hpp>
#include <utility/sdl_helpers.hpp>
#include <utility/time.hpp>
#include <queue>

class Music
{
public:
    static std::shared_ptr<Music> Create(const std::string& path)
    {
        auto* music = Mix_LoadMUS(path.c_str());
        NullCheck(music);

        auto out = std::make_shared<Music>();
        out->handle = decltype(out->handle) { music };
        return out;
    }

    Mix_Music* GetHandle() const { return handle.get(); }

private:
    sdl::UniquePtr<Mix_Music, Mix_FreeMusic> handle {};
};

struct PlaylistEntry
{
    std::shared_ptr<Music> music_handle {};
    DeltaMS fade_in_length {};
    DeltaMS fade_out_length {};
};

class MusicChannel
{
public:
    static void Init()
    {
        Mix_HookMusicFinished(MusicChannel::TrackEndCallback);
    }

    static void Free()
    {
        Mix_HaltMusic();
        Mix_HookMusicFinished(nullptr);

        current_entry = {};
        playlist = {};
    }

    static void PushTrack(const PlaylistEntry& track)
    {
        std::scoped_lock<std::mutex> lock { queue_mutex };

        if (current_entry.music_handle == nullptr)
        {
            current_entry = track;

            Mix_FadeInMusic(
                current_entry.music_handle->GetHandle(),
                -1,
                current_entry.fade_in_length.count() * 10.0f);
        }
        else
        {
            playlist.push(track);
        }
    }

    static void NextTrack(bool skip_fadeout)
    {
        std::scoped_lock<std::mutex> lock { queue_mutex };
        if (skip_fadeout)
        {
            Mix_HaltMusic();
        }
        else
        {
            Mix_FadeOutMusic(current_entry.fade_out_length.count() * 10.0f);
        }
    }

private:
    static SDLCALL void TrackEndCallback()
    {
        std::scoped_lock<std::mutex> lock { queue_mutex };

        if (playlist.empty())
        {
            current_entry = {};
            return;
        }

        Log("[MIX] Playing next track");
        current_entry = playlist.front();
        playlist.pop();

        Mix_FadeInMusic(
            current_entry.music_handle->GetHandle(),
            -1,
            current_entry.fade_in_length.count() * 10.0f);
    }

    static inline std::mutex queue_mutex {};
    static inline std::queue<PlaylistEntry> playlist {};
    static inline PlaylistEntry current_entry {};
};