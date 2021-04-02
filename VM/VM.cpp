#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

bool zero_flag(unsigned char reg)
{
    if (reg == 0) return true;
    else return false;
}


int main()
{
    unsigned char regs[16];
    unsigned char prog_mem[256];
    bool eof_flag = false;
    bool flag = false;
    bool on = true;
    int rx(0), ry(0), binCount(0);
    unsigned char temp;

    string txt1, txt2;
    cout << "Enter .bin encryption/decryption file name: ";
    cin >> txt1;
    txt1 += ".bin";
    system("cls");
    cout << "Enter file name that you want to encrypt/decrypt: ";
    cin >> txt2;
    txt2 += ".txt";
    system("cls");

    ifstream binFile(txt1.c_str(), ios::binary);
    ifstream encrFile(txt2.c_str());
    ofstream rez("result.txt");

    if (!encrFile || !binFile)
    {
        cout << "Cannot open the file";
        return 0;
    }
    while(!binFile.eof())
    {
        binFile >> noskipws >>  prog_mem[binCount];
        binCount++;
    }
    binFile.close();
    binCount--;

    cout << "File is being encrypted/crypted..." << endl;
    int n(0);
    while (on) {   
        switch (prog_mem[n])
        {
        case 0x01: // INC
            rx = (prog_mem[n+1] & 0x0f);
            regs[rx] += 1;
            n += 2;
            flag = zero_flag(regs[rx]);
            break;

        case 0x02: // DEC
            rx = (prog_mem[n+1] & 0x0f);
            regs[rx] -= 1;
            n += 2;
            flag = zero_flag(regs[rx]);
            break;

        case 0x03: // MOV
            rx = (prog_mem[n + 1] & 0x0f);
            ry = (prog_mem[n + 1] >> 4);
            regs[rx] = regs[ry];
            n += 2;
            flag = zero_flag(regs[rx]);
            break;

        case 0x04: // MOVC
            regs[0] = (int)prog_mem[n+1];
            n += 2;
            flag = zero_flag(regs[0]);
            break;

        case 0x05: // LSL
            rx = (prog_mem[n + 1] & 0x0f);
            regs[rx] = (regs[rx] << 1);
            n += 2;
            flag = zero_flag(regs[rx]);
            break;

        case 0x06: // LSR
            rx = (prog_mem[n + 1] & 0x0f);
            regs[rx] = (regs[rx] >> 1);
            n += 2;
            flag = zero_flag(regs[rx]);
            break;

        case 0x07: // JMP
            n += (char)prog_mem[n + 1];
            if (n > binCount) n -= binCount;
            if (n < 0) n += binCount;
            break;

        case 0x08: // JZ
            if (flag)
            {
                n += (char)prog_mem[n + 1];
                if (n > binCount) n -= binCount;
                if (n < 0) n += binCount;
            }
            else n += 2;
            break;

        case 0x09: // JNZ
            if (!flag)
            {
                n += (char)prog_mem[n + 1];
                if (n > binCount) n -= binCount;
                if (n < 0) n += binCount;
            }
            else n += 2;
            break;

        case 0x0a: // JFE
            if (eof_flag)
            {
                n += (char)prog_mem[n + 1];
                if (n > binCount) n -= binCount;
                if (n < 0) n += binCount;
                eof_flag = false;
            }
            else n += 2;
            break;

        case 0x0b: // RET
            on = false;
            break;

        case 0x0c: // ADD
            rx = (prog_mem[n + 1] & 0x0f);
            ry = (prog_mem[n + 1] >> 4);
            regs[rx] = regs[rx] + regs[ry];
            n+=2;
            flag = zero_flag(regs[rx]);
            break;

        case 0x0d: // SUB
            rx = (prog_mem[n + 1] & 0x0f);
            ry = (prog_mem[n + 1] >> 4);
            regs[rx] = regs[rx] - regs[ry];
            n += 2;
            flag = zero_flag(regs[rx]);
            break;

        case 0x0e: // XOR
            rx = (prog_mem[n + 1] & 0x0f);
            ry = (prog_mem[n + 1] >> 4);
            regs[rx] = (regs[rx] ^ regs[ry]);
            n += 2;
            flag = zero_flag(regs[rx]);
            break;

        case 0x0f: // OR
            rx = (prog_mem[n + 1] & 0x0f);
            ry = (prog_mem[n + 1] >> 4);
            regs[rx] = (regs[rx] | regs[ry]);
            n += 2;
            flag = zero_flag(regs[rx]);
            break;

        case 0x10: // IN
            encrFile >> noskipws >> temp;
            rx = (prog_mem[n + 1] & 0x0f);
            regs[rx] = (int)temp;
            if (encrFile.eof()) eof_flag = true;
            n += 2;
            break;

        case 0x11: // OUT
            rx = (prog_mem[n + 1] & 0x0f);
            rez << regs[rx];
            n += 2;
            break;

        default:
            system("cls");
            cout << "An unrecognized command was found.";
            return 0;
        }
    }
    system("cls");
    cout << "File was encrypted/decrypted! You can check the results in result.txt!";
    return 0;
}
