#include "Chip8.h"
#include <cmath>

void CPU::init() {
    PC = 0x200;
    SP = 0;
    DTM = 0;
    STM  = 0;
    PL = 0xFFF;
    I = 0;
    for(int i = 0; i < 0x10; i++)
        V[i] = 0;
    for(int i = 0; i < 0x10; i++)
        S[i] = 0;
}
uint16_t CPU::getProgramCounter() { return PC; }
void CPU::setProgramCounter(uint16_t addr) { PC = addr; }
void CPU::advanceProgramCounter() { if(PC < PL) PC += 2; }
void CPU::setProgramLimit(uint16_t addr) { PL = addr; }
uint16_t CPU::getStackPointer() { if(SP > 0) return S[SP-1]; else return 0; }
void CPU::setStackPointer(uint16_t ptr) { S[SP] = ptr; }
void CPU::advanceStack() { SP++; }
void CPU::popStack() { SP--; }
uint8_t CPU::getRegister(uint8_t index) { return V[index]; }
void CPU::setRegister(uint8_t index, uint8_t val) { V[index] = val; }
uint16_t CPU::getI() { return I; }
void CPU::setI(uint16_t value) { I = value; }
uint8_t CPU::getDelayTimer() { return DTM; }
void CPU::setDelayTimer(uint8_t val) { DTM = val; }
uint8_t CPU::getSoundTimer() { return STM; }
void CPU::setSoundTimer(uint8_t val) { STM = val; }
void CPU::evaluate(unsigned char x, unsigned char y, unsigned char op) {
    switch (op) {
    case 0x0: //Vx = Vy
        V[x] = V[y];
        break;
    case 0x1: //Vx | Vy
        V[x] = V[x] | V[y];
        break;
    case 0x2: //Vx & Vy
        V[x] = V[x] & V[y];
        break;
    case 0x3: //Vx ^ Vy
        V[x] = V[x] ^ V[y];
        break;
    case 0x4: { //Vx += Vy
        unsigned short s;
        s = V[x] + V[y];
        if (s >= 256)
            V[0xF] = 1;
        else
            V[0xF] = 0;
        V[x] += V[y];
        break; }
    case 0x5: { //Vx -= Vy
        short s;
        s = V[x] - V[y];
        (s < 0) ? V[0xF] = 0 : V[0xF] = 1;
        V[x] -= V[y];
        break; }
    case 0x6: //Vx >> 1
        V[0xF] = V[y] % 2;
        V[x] = V[y] >> 1;
        break;
    case 0x7: { //Vx = Vy - Vx
        V[0xF] = V[y] % 2;
        short s;
        s = V[y] - V[x];
        if(s < 0) V[0xF] = 0; else V[0xF] = 1;
        V[x] = V[y] - V[x];
        break; }
    case 0xE: //Vx << 1
        V[0xF] = V[y] % (0x80);
        V[x] = V[y] << 1;
        break;
    default:
        break;
    }
}
void CPU::updateTimers() {
    if(DTM > 0)
        DTM--;
    if(STM > 0)
        STM--;
}


uint8_t* RAM::get(uint16_t index) {
    if(index >= 4096)
        return &memory[0];
    return &memory[index];
}
uint16_t RAM::getWord(uint16_t index) {
    return (uint16_t)((memory[index] << 8) | memory[index+1]);
}
uint8_t RAM::at(uint16_t index) {
    if(index >= 4096)
        return 0;
    return memory[index];
}
void RAM::write(uint16_t index, uint8_t* vals, uint16_t size) {
    for(unsigned i = 0; i< size; i++)
        memory[index+i] = (uint8_t)(vals[i]);
}
void RAM::write(uint16_t index, uint8_t val) {
    memory[index] = val;
}
void RAM::writeWord(uint16_t index, uint16_t value) {
    memory[index] = (uint8_t)(value >> 8);
    memory[index+1] = (uint8_t)(value & 0xFF);
}
void RAM::clear() {
    for(int i = 0; i < 4096; i++)
        memory[i] = 0;
}

constexpr long double sq(long double __x) {
    if(std::sin(__x) > 0)
        return 1;
    else
        return -1;
}

Speaker::Speaker() {}

bool Speaker::init() {
    const unsigned AMPLITUDE = 8000;
    const unsigned SAMPLE_RATE = 44100;
    //const unsigned FREQUENCY = 392;
    const double TWO_PI = 6.28318;
	const double increment = 440. / 44100;
	double x = 0;
	for (unsigned i = 0; i < SAMPLES; i++) {
		raw[i] = AMPLITUDE * sq(x*TWO_PI);
		x += increment;
	}

	if (!sndbuffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
		std::cerr << "Loading failed!" << std::endl;
		return false;
	}
	snd.setBuffer(sndbuffer);
	snd.setLoop(true);
	return true;
}

bool Speaker::play() {
    if(snd.getStatus() != sf::Sound::Playing) {
        snd.setPlayingOffset(sf::Time::Zero);
        snd.play();
    }
    return true;
}

void Speaker::stop() {
    if(snd.getStatus() == sf::Sound::Playing)
        if(snd.getPlayingOffset() < sf::milliseconds(100))
            snd.stop();
}

void GPU::init() {
    va = sf::VertexArray(sf::Quads, 4 * 32 * 64);
    for(int yy = 0; yy < 32; yy++) {
        for(int xx = 0; xx < 64; xx++) {
            int pos = xx + (64 * yy);
            sf::Vertex* v = &va[(4 * pos)];
            v[0].position = sf::Vector2f(xx,yy);
            v[1].position = sf::Vector2f(xx+1,yy);
            v[2].position = sf::Vector2f(xx+1,yy+1);
            v[3].position = sf::Vector2f(xx,yy+1);
            v[0].color = sf::Color::Black;
            v[1].color = sf::Color::Black;
            v[2].color = sf::Color::Black;
            v[3].color = sf::Color::Black;
        }
    }
}

void GPU::updateScreen() {
    for(int yy = 0; yy < 32; yy++) {
        for(int xx = 0; xx < 64; xx++) {
            int pos = xx + (64 * yy);
            sf::Color newColor = sf::Color::Black;
            if(getPixel(xx,yy) == 1) {
                newColor = sf::Color::White;
            }
            sf::Vertex* v = &va[(4 * pos)];
            v[0].position = sf::Vector2f(xx,yy);
            v[1].position = sf::Vector2f(xx+1,yy);
            v[2].position = sf::Vector2f(xx+1,yy+1);
            v[3].position = sf::Vector2f(xx,yy+1);
            v[0].color = newColor;
            v[1].color = newColor;
            v[2].color = newColor;
            v[3].color = newColor;
        }
    }
}

void GPU::render(sf::RenderTexture* rt) {
    rt->draw(va);
}

void GPU::clearScreen() {
    for(int yy = 0; yy < 32; yy++)
        for(int xx = 0; xx < 64; xx++)
            screen[xx][yy] = 0;
}
uint8_t GPU::getPixel(uint8_t x, uint8_t y) {
    return screen[x][y];
}
void GPU::togglePixel(uint8_t x, uint8_t y) {
    screen[x][y] ^= 0x1;
}

bool InputHandler::getKey(uint8_t index) {
    return k[index];
}
void InputHandler::updateKeys() {
    for(int i = 0; i < 16; i++) {
        kpr[i] = k[i];
        k[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap[i]);
        if(k[i] && !kpr[i] && interrupt == -2)
            interrupt = i;
    }
}
void InputHandler::resetInterrupt() {
    interrupt = -1;
}
void InputHandler::setInterrupt() {
    interrupt = -2;
}
int8_t InputHandler::getInterrupt() {
    return interrupt;
}

void Console::init() {
    cpu.init();
    ram.clear();
    ram.write(0x50, (uint8_t*)&fontset[0], 80);
    input.updateKeys();
    input.updateKeys();
    input.resetInterrupt();
    gpu.clearScreen();
    gpu.init();
    spk.stop();
    if(!spk.init())
        _exit(-1);
    loadFlag = false;
    drawFlag = false;
}
void Console::load(std::istream& is) {
    is.seekg(0,std::ios::end);
    size_t s = is.tellg();
    is.seekg(0,std::ios::beg);
    while(!is.eof())
        is.read((char*)ram.get(0x200), s);
    if(is)
        throw std::exception();
    loadFlag = true;
    cpu.setProgramLimit(0x200 + s);
}
void Console::update(uint64_t dt) {
    uint64_t ddt = dt;
    do {
        cycle();
        ddt--;
    } while(ddt > 0);
}
void Console::cycle() {
    input.updateKeys();
    uint8_t a = ram.at(cpu.getProgramCounter());
    uint8_t b = ram.at(cpu.getProgramCounter() + 1);
    uint16_t c = (a << 8) | b;
    execute(c);
    if(cpu.getSoundTimer() == 0)
        spk.stop();
    if(cpu.getSoundTimer() >= 1)
        spk.play();
    cpu.updateTimers();
}

void Console::execute(uint16_t instr) {
    switch(instr & 0xF000) {
        case 0x0000:
            switch(instr & 0x00FF) {
                case 0xE0:
                    gpu.clearScreen();
                    cpu.advanceProgramCounter();
                    break;
                case 0xEE:
                    cpu.setProgramCounter(cpu.getStackPointer());
                    cpu.popStack();
                    cpu.advanceProgramCounter();
                    break;
                default:
                    cpu.setI(63210);
                    cpu.advanceProgramCounter();
                    break;
            }
            break;
        case 0x1000:
            cpu.setProgramCounter(instr & 0xFFF);
            break;
        case 0x2000:
            cpu.setStackPointer(cpu.getProgramCounter());
            cpu.advanceStack();
            cpu.setProgramCounter(instr & 0xFFF);
            break;
        case 0x3000:
            {
                uint8_t x = (instr & 0x0F00) >> 8;
                uint8_t N = instr & 0xFF;
                if(cpu.getRegister(x) == N)
                    cpu.advanceProgramCounter();
            }
            cpu.advanceProgramCounter();
            break;
        case 0x4000:
            {
                uint8_t x = (instr & 0x0F00) >> 8;
                uint8_t N = instr & 0xFF;
                if(cpu.getRegister(x) != N)
                    cpu.advanceProgramCounter();
            }
            cpu.advanceProgramCounter();
            break;
        case 0x5000:
            {
                uint8_t x = (instr & 0x0F00) >> 8;
                uint8_t y = (instr & 0xF0) >> 4;
                if(cpu.getRegister(x) == cpu.getRegister(y))
                    cpu.advanceProgramCounter();
            }
            cpu.advanceProgramCounter();
            break;
        case 0x6000:
            {
                uint8_t x = (instr & 0x0F00) >> 8;
                uint8_t N = instr & 0xFF;
                cpu.setRegister(x, N);
            }
            cpu.advanceProgramCounter();
            break;
        case 0x7000:
            {
                uint8_t x = (instr & 0x0F00) >> 8;
                uint8_t N = instr & 0xFF;
                uint8_t Vx = cpu.getRegister(x);
                if((short)Vx + (short)N > 0xFF)
                    cpu.setRegister(0xF, 1);
                Vx += N;
                cpu.setRegister(x, Vx);
            }
            cpu.advanceProgramCounter();
            break;
        case 0x8000:
            {
                uint8_t x = (instr & 0xF00) >> 8;
                uint8_t y = (instr & 0x0F0) >> 4;
                uint8_t op = (instr & 0x00F);
                cpu.evaluate(x, y, op);
            }
            cpu.advanceProgramCounter();
            break;
        case 0x9000:
            {
                uint8_t x = (instr & 0x0F00) >> 8;
                uint8_t y = (instr & 0xF0) >> 4;
                if(cpu.getRegister(x) != cpu.getRegister(y))
                    cpu.advanceProgramCounter();
            }
            cpu.advanceProgramCounter();
            break;
        case 0xA000:
            cpu.setI(instr & 0xFFF);
            cpu.advanceProgramCounter();
            break;
        case 0xB000:
            cpu.setProgramCounter((instr & 0xFFF) + cpu.getRegister(0));
            //cpu.advanceProgramCounter();
            break;
        /**Set Vx to Random**/
        case 0xC000:
            {
                uint8_t x = (instr & 0x0F00) >> 8;
                uint8_t nn = (instr & 0x00FF);
                cpu.setRegister(x, rand() & nn);
            }
            cpu.advanceProgramCounter();
            break;
        /**Draws to the screen**/
        case 0xD000:
            {
                uint8_t x = cpu.getRegister((instr & 0x0F00) >> 8);
                uint8_t y = cpu.getRegister((instr & 0x00F0) >> 4);
                uint8_t h = (instr & 0x000F);

                uint16_t pixel = 0;

                cpu.setRegister(0xF, 0);
                for (int yline = 0; yline < h; yline++)
                {
                    pixel = ram.at(cpu.getI() + yline);
                    for (int xline = 0; xline < 8; xline++)
                    {
                        if ((pixel & (0x80 >> xline)) != 0)
                        {
                            if (gpu.getPixel(x + xline, y + yline) == 1)
                                cpu.setRegister(0xF, 1);
                            gpu.togglePixel(x + xline, y + yline);
                        }
                    }
                }
                drawFlag = true;
            }
            cpu.advanceProgramCounter();
            break;
        /**Key handling functions**/
        case 0xE000:
            switch (instr & 0x00FF) {
                /**Skip if key is pressed**/
                case 0x9E:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        uint8_t Vx = cpu.getRegister(x);
                        if(input.getKey(Vx))
                            cpu.advanceProgramCounter();
                    }
                    cpu.advanceProgramCounter();
                    break;
                /**Skip if key is not pressed**/
                case 0xA1:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        uint8_t Vx = cpu.getRegister(x);
                        if(!input.getKey(Vx))
                            cpu.advanceProgramCounter();
                    }
                    cpu.advanceProgramCounter();
                    break;
                default:
                    cpu.advanceProgramCounter();
                    break;
            }
            break;
        case 0xF000:
            switch (instr & 0x00FF) {
                /**Set Vx to Delay**/
                case 0x07:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        cpu.setRegister(x, cpu.getDelayTimer());
                    }
                    cpu.advanceProgramCounter();
                    break;
                /**Skip if key is not pressed**/
                case 0x0A:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        switch(input.getInterrupt()) {
                            case -2:
                                break;
                            case -1:
                                input.setInterrupt();
                                break;
                            default:
                                cpu.setRegister(x, input.getInterrupt());
                                cpu.advanceProgramCounter();
                                input.resetInterrupt();
                                break;
                        }
                    }
                    break;
                case 0x15:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        cpu.setDelayTimer(cpu.getRegister(x));
                    }
                    cpu.advanceProgramCounter();
                    break;
                case 0x18:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        cpu.setSoundTimer(cpu.getRegister(x));
                    }
                    cpu.advanceProgramCounter();
                    break;
                /** I = I + Vx **/
                case 0x1E:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        cpu.setI(cpu.getI() + cpu.getRegister(x));
                    }
                    cpu.advanceProgramCounter();
                    break;
                /** I = F **/
                case 0x29:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        uint8_t Vx = cpu.getRegister(x);
                        cpu.setI(0x50 + (5*Vx));
                    }
                    cpu.advanceProgramCounter();
                    break;
                /** [I] = B **/
                case 0x33:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        uint8_t Vx = cpu.getRegister(x);
                        uint16_t I = cpu.getI();
                        ram.write(I, Vx / 100);
                        ram.write(I+1, (Vx / 10) % 10);
                        ram.write(I+2, (Vx % 100) % 10);
                    }
                    cpu.advanceProgramCounter();
                    break;
                /** V0-Vx = [I] **/
                case 0x55:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        uint16_t I = cpu.getI();
                        for(unsigned i = 0; i <= x; i++) {
                            uint8_t Vi = cpu.getRegister(i);
                            ram.write(I+i, Vi);
                        }
                    }
                    cpu.advanceProgramCounter();
                    break;
                /** V0-Vx = [I] **/
                case 0x65:
                    {
                        uint8_t x = (instr & 0x0F00) >> 8;
                        uint16_t I = cpu.getI();
                        for(unsigned i = 0; i <= x; i++) {
                            cpu.setRegister(i, ram.at(I+i));
                        }
                    }
                    cpu.advanceProgramCounter();
                    break;
                default:
                    cpu.advanceProgramCounter();
                    break;
            }
            break;
        default:
            cpu.advanceProgramCounter();
            break;
    }

}

void Console::render(sf::RenderTexture* rt) {
    /*for(int yy = 0; yy < 32; yy++) {
        for(int xx = 0; xx < 64; xx++) {
            if(gpu.getPixel(xx, yy) == 1) {
                sf::RectangleShape rs;
                rs.setPosition(xx, yy);
                rs.setSize(sf::Vector2f(1,1));
                rs.setFillColor(sf::Color::White);
                rt->draw(rs);
            }
        }
    }*/
    gpu.updateScreen();
    gpu.render(rt);
    drawFlag = false;
}
#ifdef WRITE_DEBUG
void Console::debug(std::stringstream& out) {
    out << "PC:0x" << std::hex << cpu.getProgramCounter() << std::dec;
    out << "|I:" << cpu.getI();
    out << "|SP:0x" << std::hex << cpu.getStackPointer() << std::dec;
    out << "|DT:" << (uint16_t)cpu.getDelayTimer();
    out << "|ST:" << (uint16_t)cpu.getSoundTimer() << "\n";
    for(int i = 0; i < 0x8; i++)
        out << " V" << std::hex << i << std::dec << ":" << (uint16_t)cpu.getRegister(i);
    out << "\n";
    for(int i = 0x8; i < 0x10; i++)
        out << " V" << std::hex << i << std::dec << ":" << (uint16_t)cpu.getRegister(i);
    out << "\n";
    /*out << "RAM:" << std::hex;
    for(int i = 0x200; i < 0x300; i++) {
        if(i > 0x200) out << ", ";
        if(i > 0x200 && i % 0x10 == 0) out << "\n";
        out << (uint16_t)ram.at(i);
    }*/
    out << std::dec << "\n";
}
#endif
