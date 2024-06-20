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

    // ΖΗΤΟΥΜΕΝΕΣ ΣΥΝΑΡΤΗΣΕΙΣ

    // Συνάρτηση υπολογισμού Παραγωντικού
    HugeInt factorial() {
        if (size == 0 || (size == 1 && Digits[0] == '0')) {
            allocateDigits(1);
            Digits[0] = '1';
            return *this;
        }

        unsigned int n = 0;
        for (unsigned int i = 0; i < size; i++)
            n = n * 10 + (Digits[size - 1 - i] - '0');

        // 'log10(2 * M_PI * n) / 2' Αυτή η έκφραση υπολογίζει το λογαριθμικό μέρος της σταθεράς για τον υπολογισμό των ψηφίων του παραγοντικού, βάσει της προσέγγισης Stirling.
        // 'n * log10(n / M_E)' Αυτή η έκφραση υπολογίζει το κύριο μέρος του λογαριθμικού υπολογισμού, όπου n είναι το αριθμητικό μέρος του παραγοντικού και M_E είναι η σταθερά e
        unsigned int numDigits = static_cast<unsigned int>(log10(2 * M_PI * n) / 2 + n * log10(n / M_E)) + 1;
        allocateDigits(numDigits);

        HugeInt result("1"); // Δημιουργία καινούργιου αντικειμένου που θα χρησιμοποιηθεί αργότερα.
        // Ο υπολογισμός ξεκινάει από το 3ο ψηφίο (το 2) γιατί είναι τα προηγούμενα 2 είναι τα 0 και 1 όπου ισχύει:
        // !0=1, !1=1
        for (unsigned int i = 2; i <= n; i++) {
            HugeInt multiplier(to_string(i));
            result = result * multiplier;
        }

        *this = result; // Τοποθέτηση νέων δεδομένων στο καλών αντικείμενο
        return *this;
    }

    HugeInt& power(unsigned int B) {
        // Εφαρμόσουμε τον αλγόριθμο της ταχείας δύναμης
        if (B == 0) {
            // Any number to the power of 0 is 1
            allocateDigits(1);
            Digits[0] = '1';
            return *this;
        }

        HugeInt base(*this); // Αντίγραφο της βάσης
        HugeInt result("1"); // Αρχικοποίηση του αποτελέσματος σε 1

        // Η ιδέα είναι ότι χρησιμοποιούμε το B μέχρι να μηδενιστεί
        // Αν B είναι περιττός, τότε πολλαπλασιάζουμε τον αποτέλεσμα με 1 στην αρχή και συνεχίζουμε σαν να είναι πλέον άρτιος

        // Το κόλπο για να, ΜΕΙΩΘΕΙ Ο ΑΡΙΘΜΟΣ ΤΩΝ ΠΟΛΛΑΠΛΑΣΙΑΣΜΩΝ είναι το εξής:
        // Πολλαπλασιάζουμε κάθε φορά όχι με τη βάση, αλλά με το αποτέλεσμα που δίνει η βάση σε κάθε πολλαπλασιασμό ξεκινώντας από την αρχική τιμή που δώθηκε γι'αυτή.
        // ΑΝ έχει ΠΕΡΙΤΤΟ αριθμό η βάση τότε στη πρώτη επανάληψη του βρόγχου 'while' θέτει το αποτέλεσμα στην τιμή της βάσης σαν να λέμε πχ Α=5, Β=5: res=5^1 και μετά προσθέτει σε αυτό τα:
        // [Α1=(5*5) * Α2=(5*5)] = 25*25 = 625
        // -----------------------------------------------------+
        // αρχικές τιμές: A=5, Β=5, result=1
        //
        // Στο ΤΕΛΟΣ του κάθε βρόγχου θα ισχύουν:
        // 1ος βρόγχος Α=25, B=2, result=5
        // 2ος βρόγχος Α=625, B=1, result=5
        // 3ος βρόγχος Α=3125, B=0, result=3125
        while (B > 0) {
            if (B % 2 == 1) {
                result = result * base;
            }
            base = base * base;
            B /= 2;
        }

        // Αντιγραφή του αποτελέσματος στο τρέχον αντικείμενο
        *this = result;
        return *this;
    }
};

// Αρχικοποίηση στατικού μέλους
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
//Αυτό μπορεί να είναι χρήσιμο για να ελέγξουμε αν ένα αντικείμενο είναι "έγκυρο" ή αν έχει κάποια συγκεκριμένη κατάσταση.
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

        newDigits[size] = carry + '0';
        delete[] Digits;
        Digits = newDigits;
        size++;
    }
    return *this;
}

// "*" overload
HugeInt HugeInt::operator*(const HugeInt& obj2) const {
    int maxSize = size + obj2.size;

    char* result = new char[maxSize + 1];
    for (int i = 0; i < maxSize; i++)
        result[i] = '0';

    for (int i = 0; i < size; i++) {
        int carry = 0;
        for (int j = 0; j < obj2.size || carry; j++) {
            int mul = (result[i + j] - '0') + (Digits[i] - '0') * (j < obj2.size ? obj2.Digits[j] - '0' : 0) + carry;
            carry = mul / 10;
            result[i + j] = (mul % 10) + '0';
        }
    }

    reverse(result, result + maxSize);
    string result_conv = convertToString(result);
    delete[] result;  // Clean up allocated memory
    return HugeInt(result_conv);
}

// "+" overloard
HugeInt HugeInt::operator+(const HugeInt& obj2) const {
    int carry = 0;
    int maxSize = max(size, obj2.size);

    char* result = new char[maxSize + 1];

    for (int i = 0; i < maxSize || carry; i++) {
        int sum = carry;

        if (i < size)
            sum += Digits[i] - '0';
        if (i < obj2.size)
            sum += obj2.Digits[i] - '0';

        carry = sum / 10;
        result[i] = (sum % 10) + '0';
    }

    reverse(result, result + maxSize);
    string result_conv = convertToString(result);
    delete[] result;  // Clean up allocated memory
    return HugeInt(result_conv);
}

// copy constructor
HugeInt::HugeInt(const HugeInt& rv) {
    size = rv.size;
    Digits = new char[size];
    for (int i = 0; i < size; i++)
        Digits[i] = rv.Digits[i];
    activeInstances++;
}

// destructor
HugeInt::~HugeInt() {
    activeInstances--;
    delete[] Digits;
}

// overload constructor that takes string
HugeInt::HugeInt(string numStr) {
    size = numStr.length();
    Digits = new char[size];
    for (int i = 0; i < size; i++)
        Digits[i] = numStr[size - i - 1];
    activeInstances++;
}

// default constructor
HugeInt::HugeInt() {
    size = 1;
    Digits = new char[size];
    Digits[0] = '0';
    activeInstances++;
}

int main() {
    {
        ΗugeInt num1("5");
        cout << "Active Instances: " << HugeInt::getActiveIntstances() << endl;
        HugeInt num2("23946687");
        cout << "Active Instances: " << HugeInt::getActiveIntstances() << endl;
        HugeInt num3;
        cout << "Active Instances: " << HugeInt::getActiveIntstances() << endl;

    /*
        num3 = num1 + num2;
        num3.printDigitsAsIntegers();

        num3 = num1 * num2;
        num3.printDigitsAsIntegers();

        num3 = ++num1;
        num3.printDigitsAsIntegers();

        num3 = num2++;
        num3.printDigitsAsIntegers();

        num3.printStuff(num1, num2);


        num1.factorial();
        num1.printDigitsAsIntegers();


        num1.power(5);
        num1.printDigitsAsIntegers();

        // num3 = num1 * 2;
        // *ερώτηση άσκησης 4* Απάντηση: Η παραπάνω πράξη είναι αδύνατο να πραγματοποιηθεί από τη στιγμή που έχουμε υπερφορτώσει τον τελεστή "*" στη κλάση HugeInt. Αυτό σημαίνει ότι όταν περιλαμβάνεται στη πράξη αντικείμενο τύπου HugeInt με τη χρήση του τελεστή αυτού υπερκαλύπτεται από τον νατίχτοιχο constructor.

        //cout << num2;
    */

    }
    cout << "Active Instances: " << HugeInt::getActiveIntstances() << endl;

    return 0;
}

/*
// printStuff function
void HugeInt::printStuff(const HugeInt& a, const HugeInt& b) {
    cout << "Digits in a as integers: ";
    a.printDigitsAsIntegers();
    cout << "Digits in b as integers: ";
    b.printDigitsAsIntegers();
}
*/
