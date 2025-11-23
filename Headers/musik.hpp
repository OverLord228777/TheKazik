#pragma once
#include <SFML/Audio.hpp>

std::vector<std::string> playlist = {
        "Govno",
        "Povorot ne tuda",
        "Hava Nagila",
};
int current_track_index = 0;

std::string switch_next_track() {
    // ”величиваем индекс на 1, затем берем остаток от делени€ на размер плейлиста.
    //  огда current_track_index достигает последнего индекса,
    // следующее значение (current_track_index + 1) % size становитс€ 0 (первый элемент).
    current_track_index = (current_track_index + 1) % playlist.size();
    return playlist[current_track_index];
    };

class KazikMusic {
private:
    sf::Music music;
    float volume = 50.f;

public:
    bool load(const std::string& filename) {
        if (!music.openFromFile(filename)) return false;
        music.setLoop(true);
        music.setVolume(volume);
        return true;
    }

    void setVolume(float vol) {
        volume = vol;
        music.setVolume(volume);
    }

    void play() { music.play(); }
    void pause() { music.pause(); }
    void stop() { music.stop(); }
};