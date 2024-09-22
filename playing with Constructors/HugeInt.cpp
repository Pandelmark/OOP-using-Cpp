#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>

using namespace std;

// IRRELEVANT CONSTRUCTORS
string convertToString(char* a) {
    string s(a);
    return s;
}

// HugeInt CLASS
class HugeInt {
private:
    unsigned int size;
    char* Digits;
    static unsigned int activeInstances;

    void allocateDigits(unsigned int newSize) {
        char* newDigits = new char[newSize];
        for (int i = 0; i < size && i < newSize; i++)
            newDigits[i] = Digits[i];

        delete[] Digits;
        Digits = newDigits;
        size = newSize;
    }

public:
    HugeInt();
    HugeInt(string numStr);
    ~HugeInt();

    unsigned int getSize() const {
        return size;
    }

    const char* getDigits() const{
        return Digits;
    }

    static unsigned int getActiveIntstances(){
        return activeInstances;
    }

    void printDigitsAsIntegers() const {
        for (int i = size - 1; i >= 0; i--)
            cout << (Digits[i] - '0') << " ";
        cout << endl;
    }

    HugeInt(const HugeInt&);
    HugeInt operator+(const HugeInt&) const;
    HugeInt operator*(const HugeInt&) const;
    HugeInt& operator++();
    HugeInt operator++(int);
    HugeInt& operator=(const HugeInt&);
    bool operator<(const HugeInt&) const;
    bool operator>(const HugeInt&) const;
    bool operator!=(const HugeInt&) const;
    bool operator==(const HugeInt&) const;
    bool operator>=(const HugeInt&) const;
    bool operator<=(const HugeInt&) const;
    explicit operator bool() const;
    static int getInstance();

    void printStuff(const HugeInt&, const HugeInt&);

    // REQUIRED FUNCTIONS

    // Factorial calculation function
    HugeInt factorial() {
        if (size == 0 || (size == 1 && Digits[0] == '0')) {
            allocateDigits(1);
            Digits[0] = '1';
            return *this;
        }

        unsigned int n = 0;
        for (unsigned int i = 0; i < size; i++)
            n = n * 10 + (Digits[size - 1 - i] - '0');

        // 'log10(2 * M_PI * n) / 2' This expression calculates the logarithmic part of the constant for digit calculation of the factorial based on Stirling's approximation.
        // 'n * log10(n / M_E)' This expression calculates the main part of the logarithmic calculation, where n is the numeric part of the factorial and M_E is the constant e.
        unsigned int numDigits = static_cast<unsigned int>(log10(2 * M_PI * n) / 2 + n * log10(n / M_E)) + 1;
        allocateDigits(numDigits);

        HugeInt result("1"); // Creating a new object to be used later.
        // Calculation starts from the 3rd digit (2) because the previous 2 are 0 and 1 where:
        // !0=1, !1=1
        for (unsigned int i = 2; i <= n; i++) {
            HugeInt multiplier(to_string(i));
            result = result * multiplier;
        }

        *this = result; // Place new data in the calling object.
        return *this;
    }

    HugeInt& power(unsigned int B) {
        // We apply the fast exponentiation algorithm.
        if (B == 0) {
            // Any number to the power of 0 is 1.
            allocateDigits(1);
            Digits[0] = '1';
            return *this;
        }

        HugeInt base(*this); // Copy of the base.
        HugeInt result("1"); // Initialize result to 1.

        // The idea is to use B until it becomes zero.
        // If B is odd, then we multiply the result by 1 at the beginning and continue as if it is now even.

        // The trick to REDUCE THE NUMBER OF MULTIPLICATIONS is as follows:
        // Each time we multiply not with the base, but with the result of squaring the base from the initial value.
        // IF the base has an ODD value, then in the first iteration of the loop 'while', it sets the result to the value of the base as if we say e.g. A=5, B=5: res=5^1, and then adds to it:
        // [A1=(5*5) * A2=(5*5)] = 25*25 = 625
        // -----------------------------------------------------+
        // Initial values: A=5, B=5, result=1
        //
        // At the END of each loop, the following will hold:
        // 1st loop A=25, B=2, result=5
        // 2nd loop A=625, B=1, result=5
        // 3rd loop A=3125, B=0, result=3125
        while (B > 0) {
            if (B % 2 == 1) {
                result = result * base;
            }
            base = base * base;
            B /= 2;
        }

        // Copy result to current object.
        *this = result;
        return *this;
    }
};

// Initialize static member
unsigned int HugeInt::activeInstances=0;

// "cout <<" overload
ostream& operator<<(ostream& channel, const HugeInt& x) {
        channel << "object HugeInt, size:" << x.getSize()<< endl;
            return channel;
}

// OVERLOAD CONSTRUCTORS


// "bool()" overload
HugeInt::operator bool() const {
    for (int i = 0; i < size; i++)
        if (Digits[i] != '0')
            return true;
    return false;
}

// LESS-OR-EQUAL "<=" overload
bool HugeInt::operator<=(const HugeInt& other) const {
    if (size != other.size)
        return size < other.size;

    for (int i = size - 1; i >= 0; i--) {
        if (Digits[i] > other.Digits[i])
            return false;
        else if (Digits[i] < other.Digits[i])
            return true;
    }
    return true;
}

// GREATER-OR-EQUAL ">=" overload
bool HugeInt::operator>=(const HugeInt& other) const {
    if (size != other.size)
        return size > other.size;

    for (int i = size - 1; i >= 0; i--) {
        if (Digits[i] > other.Digits[i])
            return true;
        else if (Digits[i] < other.Digits[i])
            return false;
    }
    return true;
}

// EQUAL "==" overload
bool HugeInt::operator==(const HugeInt& other) const {
    if (size != other.size)
        return false;
    for (int i = size - 1; i >= 0; i--)
        if (Digits[i] != other.Digits[i])
            return false;
    return true;
}

// NOT-EQUAL "!=" overload
bool HugeInt::operator!=(const HugeInt& other) const {
    return !(*this == other);
}

// GREATER-THAN ">" overload
bool HugeInt::operator>(const HugeInt& other) const {
    if (size != other.size)
        return size > other.size;

    for (int i = size - 1; i >= 0; i--) {
        if (Digits[i] != other.Digits[i])
            return Digits[i] > other.Digits[i];
    }
    return false;
}

// LESS-THAN "<" overload
bool HugeInt::operator<(const HugeInt& other) const {
    if (size != other.size)
        return size < other.size;

    for (int i = size - 1; i >= 0; i--) {
        if (Digits[i] != other.Digits[i])
            return Digits[i] < other.Digits[i];
    }
    return false;
}

// ASSIGNMENT "=" overload
HugeInt& HugeInt::operator=(const HugeInt& other) {
    if (this == &other)
        return *this;

    delete[] Digits;

    size = other.size;
    Digits = new char[size];
    for (int i = 0; i < size; i++)
        Digits[i] = other.Digits[i];

    return *this;
}

// POSTFIX "++" overload
HugeInt HugeInt::operator++(int) {
    HugeInt temp(*this);
    ++(*this);
    return temp;
}

// PREFIX "++" overload
HugeInt& HugeInt::operator++() {
    int carry = 1;
    for (int i = 0; i < size; i++) {
        int sum = (Digits[i] - '0') + carry;
        Digits[i] = (sum % 10) + '0';
        carry = sum / 10;

        if (carry == 0)
            break;
    }

    if (carry > 0) {
        char* newDigits = new char[size + 1];
        for (int i = 0; i < size; i++)
            newDigits[i] = Digits[i];

        newDigits[size] = '1';
        delete[] Digits;
        Digits = newDigits;
        size++;
    }

    return *this;
}

// MULTIPLICATION "*" overload
HugeInt HugeInt::operator*(const HugeInt& other) const {
    unsigned int newSize = size + other.size;
    char* newDigits = new char[newSize];
    memset(newDigits, '0', newSize);

    for (int i = 0; i < size; i++) {
        int carry = 0;
        for (int j = 0; j < other.size; j++) {
            int product = (Digits[i] - '0') * (other.Digits[j] - '0') + (newDigits[i + j] - '0') + carry;
            newDigits[i + j] = (product % 10) + '0';
            carry = product / 10;
        }

        if (carry > 0)
            newDigits[i + other.size] = carry + '0';
    }

    HugeInt result;
    result.Digits = newDigits;
    result.size = newSize;

    return result;
}

// ADDITION "+" overload
HugeInt HugeInt::operator+(const HugeInt& other) const {
    unsigned int newSize = max(size, other.size) + 1;
    char* newDigits = new char[newSize];
    memset(newDigits, '0', newSize);

    int carry = 0;
    for (int i = 0; i < newSize - 1; i++) {
        int sum = carry;
        if (i < size)
            sum += (Digits[i] - '0');
        if (i < other.size)
            sum += (other.Digits[i] - '0');
        newDigits[i] = (sum % 10) + '0';
        carry = sum / 10;
    }

    newDigits[newSize - 1] = carry + '0';

    HugeInt result;
    result.Digits = newDigits;
    result.size = newSize;

    return result;
}

// DESTRUCTOR
HugeInt::~HugeInt() {
    delete[] Digits;
}

// DEFAULT CONSTRUCTOR
HugeInt::HugeInt() : size(0), Digits(nullptr) {}

// STRING CONSTRUCTOR
HugeInt::HugeInt(string numStr) {
    size = numStr.length();
    Digits = new char[size];
    for (int i = 0; i < size; i++)
        Digits[i] = numStr[size - 1 - i];
}

// COPY CONSTRUCTOR
HugeInt::HugeInt(const HugeInt& other) {
    size = other.size;
    Digits = new char[size];
    for (int i = 0; i < size; i++)
        Digits[i] = other.Digits[i];
}
