#ifndef CHIP8_H_INCLUDED
#define CHIP8_H_INCLUDED

#include <sstream>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

const unsigned char fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const uint8_t keyMap[16] = {
    sf::Keyboard::X,
    sf::Keyboard::Num1,
    sf::Keyboard::Num2,
    sf::Keyboard::Num3,
    sf::Keyboard::Q,
    sf::Keyboard::W,
    sf::Keyboard::E,
    sf::Keyboard::A,
    sf::Keyboard::S,
    sf::Keyboard::D,
    sf::Keyboard::Z,
    sf::Keyboard::C,
    sf::Keyboard::Num4,
    sf::Keyboard::R,
    sf::Keyboard::F,
    sf::Keyboard::V
};

const unsigned SAMPLES = 44100;

class CPU {
private:
    uint16_t PC;
    uint8_t V[16];
    uint16_t S[16];
    uint16_t I;
    uint16_t SP;
    uint8_t DTM;
    uint8_t STM;
    uint16_t PL;
public:
    void init();
    uint16_t getProgramCounter();
    void setProgramCounter(uint16_t index);
    uint16_t getStackPointer();
    void setStackPointer(uint16_t ptr);
    void advanceStack();
    void popStack();
    void advanceProgramCounter();
    void setProgramLimit(uint16_t addr);
    uint16_t getI();
    void setI(uint16_t value);
    uint8_t getRegister(uint8_t index);
    void setRegister(uint8_t index, uint8_t val);
    void evaluate(unsigned char x, unsigned char y, unsigned char op);
    uint8_t getSoundTimer();
    void setSoundTimer(uint8_t val);
    uint8_t getDelayTimer();
    void setDelayTimer(uint8_t val);
    void updateTimers();
};

class RAM {
private:
    uint8_t memory[4096];
public:
    uint8_t* get(uint16_t index);
    uint16_t getWord(uint16_t index);
    uint8_t at(uint16_t index);
    void writeWord(uint16_t index, uint16_t value);
    void write(uint16_t index, uint8_t* vals, uint16_t size);
    void write(uint16_t index, uint8_t val);
    void clear();
};

class Speaker {
private:
    int16_t raw[SAMPLES];
    sf::SoundBuffer sndbuffer;
    sf::Sound snd;
public:
    Speaker();
    bool init();
    bool play();
    void stop();
};

class GPU {
private:
    uint8_t screen[64][32];
    sf::VertexArray va;
public:
    void init();
    void setPixel(uint8_t x, uint8_t y, uint8_t val);
    uint8_t getPixel(uint8_t x, uint8_t y);
    void togglePixel(uint8_t x, uint8_t y);
    uint8_t draw(uint8_t x, uint8_t y, uint8_t height);
    void clearScreen();
    void updateScreen();
    void render(sf::RenderTexture* rt);
};

class InputHandler {
private:
    bool k[16];
    bool kpr[16];
    int8_t interrupt;
public:
    bool getKey(uint8_t index);
    void updateKeys();
    int8_t getInterrupt();
    void setInterrupt();
    void resetInterrupt();
};

class Console {
private:
    CPU cpu;
    RAM ram;
    GPU gpu;
    Speaker spk;
    InputHandler input;
    bool drawFlag;
    bool pauseFlag;
    bool loadFlag;
public:
    void load(std::istream& is);
    void init();
    void update(uint64_t dt);
    void cycle();
    void execute(uint16_t instr);
    void pause() { pauseFlag = true; }
    void resume() { pauseFlag = false; }
    bool isPaused() { return pauseFlag; }
    bool isReadyToRender() { return drawFlag; }
    bool isLoaded() { return loadFlag; }
    void render(sf::RenderTexture* rt);
    #ifdef WRITE_DEBUG
    void debug(std::stringstream& out);
    #endif
};


#endif // CHIP8_H_INCLUDED
