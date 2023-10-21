#include <cstdint>
#include <type_traits>
#include <iostream>
#include <limits>

using namespace std;


/*
Constructors:
-Default constructor initializes the bitmask to zero.
-Copy constructor for creating a new bitmask from an existing one.
-Explicit constructor for initializing the bitmask with an initial value.
-Variadic template constructor for setting bits during initialization.
-ResetAllBits method to reset all bits to zero.

Bit Manipulation :
-SetBit method for setting a specific bit.
-ClearBit method for clearing a specific bit.
-ClearBits method for clearing multiple bits.
-ToggleBit method for toggling a specific bit.

Query and Information:
-Methods to check if a specific bit is set(IsBitSet) and if any bit is set(AnyBitSet).
-CountSetBits method to count the number of set bits.
-IsBitNSet method to check if a specific number of bits are set.
-AllBitsSet method to check if all bits are set.
-IsAnyBitSetInRange method to check if any bit in the current bitmask is set in another bitmask.
-toBinaryString method to obtain a binary string representation of the bitmask.

Bitwise Operations :
-Overloaded operators for bitwise OR(| ), AND(&), XOR(^), addition(+), subtraction(-), left shift(<< ), and right shift(>> ).
-Compound assignment operators for in - place modifications(+=, -=, ^=).
-Comparison operators(== and != ) for comparing bitmasks.
-Assignment operator ( = ) for assigning one bitmask to another.

Explicit Constructor :
-The constructor with an initial value is marked as explicit to prevent implicit conversions.

Specialized Template :
-The Enummask template is introduced to specialize BitMask for use with enumerations.
*/



// A base template class for common bit manipulation functionality.
template <typename MaskType, typename OpType, int TMax>
struct BitMaskBase {
    // Helper function to check if a bit position is valid.
    static constexpr bool IsBitValidPos(const int pos)
    {
        return pos >= 0 && pos <= TMax;
    }

    // Ensure that TMax is within the allowed bits in MaskType.
    static_assert(IsBitValidPos(TMax), "TMax shouldnt be above the allowed bits in MaskType");


    // Constructors and Initialization:


    // Default constructor initializes the BitMaskBase to zero.
    BitMaskBase() : Mask(0) {};

    // Copy constructor for creating a new BitMaskBase from an existing one.
    BitMaskBase(const BitMaskBase& other) : Mask(other.Mask) {}

    // Explicit constructor for initializing the BitMaskBase with an initial value.
    explicit BitMaskBase(MaskType initialValue) : Mask(initialValue) {}

    // Variadic template constructor for setting bits during compile time.
    template<typename...Args>
    constexpr BitMaskBase(const Args&...bits) : Mask(0)
    {
        (SetBits(static_cast<OpType>(bits)), ...);
    }


    // Bit Manipulation Functions:


    // Set a specific bit at position bitPos.
    template<typename...Args>
    void SetBits(const Args&... bits)
    {
        (SetBit(static_cast<OpType>(bits)), ...);
    }

    // Clear a specific bit at position bitPos.
    void SetBit(const OpType bitPos)
    {
        Mask |= (MaskType(1) << static_cast<MaskType>(bitPos));
    }

    // Clear mutliple bits.
    template <typename... Args>
    void ClearBits(const Args&... args) {
        (clearBit(args ...));
    }

    // Clear a specific bit at position bitPos.
    void ClearBit(const OpType bitPos)
    {
        Mask &= ~(MaskType(1) << static_cast<MaskType>(bitPos));
    }

    // Reset all bits to zero.
    void ResetAllBits() {
        Mask = 0;
    }

    // Toggle a specific bit at position bitPos.
    void ToggleBit(const OpType bitPos)
    {
        Mask ^= (MaskType(1) << static_cast<MaskType>(bitPos));
    }

    // Check if a specific bit at position pos is set.
    bool IsBitSet(const OpType pos) const {
        return (Mask & (MaskType(1) << static_cast<MaskType>(pos))) != 0;
    }

    // Check if any bit is set in the BitMaskBase.
    bool AnyBitSet() const {
        return Mask != 0;
    }

    // Check if any bit in the current BitMaskBase is set in another BitMaskBase.
    bool IsAnyBitSetInRange(BitMaskBase otherMask) const {
        return (Mask & otherMask) != 0;
    }

    // Check if all bits in the BitMaskBase are set.
    bool AllBitsSet() const {
        return Mask == std::numeric_limits<MaskType>::max();
    }

    // Count the number of set bits in the BitMaskBase.
    int CountSetBits() const {
        int count = 0;
        MaskType tempMask = Mask;
        while (tempMask != 0) {
            count += tempMask & 1;
            tempMask >>= 1;
        }
        return count;
    }

    // Check if a specific number of bits are set.
    OpType IsBitNSet(int pos) const {
        int count = 0;
        MaskType tempMask = Mask;
        while (tempMask != 0 && count < pos) {
            count += tempMask & 1;
            tempMask >>= 1;
        }
        return static_cast<OpType>(count);
    }

    // Convert the BitMaskBase to a binary string representation.
    std::string toBinaryString() const {
        std::string result;
        MaskType value = Mask;

        for (int i = TMax - 1; i >= 0; i--) {
            result += ((value >> i) & 1) ? '1' : '0';
        }

        return result;
    }

    // Bitwise Operations:

    BitMaskBase operator+(const BitMaskBase& other) const {
        BitMaskBase result;
        result.Mask = Mask | other.Mask;
        return result;
    }

    BitMaskBase operator-(const BitMaskBase& other) const {
        BitMaskBase result;
        result.Mask = Mask & (~other.Mask);
        return result;
    }

    BitMaskBase& operator+=(const BitMaskBase& other) {
        Mask |= other.Mask;
        return *this;
    }

    BitMaskBase& operator-=(const BitMaskBase& other) {
        Mask &= (~other.Mask);
        return *this;
    }

    BitMaskBase operator^(const BitMaskBase& other) const {
        BitMaskBase result;
        result.Mask = Mask ^ other.Mask;
        return result;
    }

    BitMaskBase& operator^=(const BitMaskBase& other) {
        Mask ^= other.Mask;
        return *this;
    }

    BitMaskBase operator~() const {
        BitMaskBase result(*this);
        result.Mask = ~result.Mask;
        return result;
    }

    BitMaskBase operator<<(int shift) const {
        BitMaskBase result(*this);
        result.Mask <<= shift;
        return result;
    }

    BitMaskBase operator>>(int shift) const {
        BitMaskBase result(*this);
        result.Mask >>= shift;
        return result;
    }

    BitMaskBase& operator<<=(int shift) {
        Mask <<= shift;
        return *this;
    }

    BitMaskBase& operator>>=(int shift) {
        Mask >>= shift;
        return *this;
    }

    bool operator==(const BitMaskBase& other) const {
        return Mask == other.Mask;
    }

    bool operator!=(const BitMaskBase& other) const {
        return Mask != other.Mask;
    }

    BitMaskBase& operator=(const BitMaskBase& other) {
        Mask = other.Mask;
        return *this;
    }

    MaskType Mask;
};

// A template class for creating and manipulating numerical bit masks.
template <typename MaskType, typename OpType = MaskType, int TMax = (sizeof(MaskType) * 8)>
struct BitMask : public BitMaskBase<MaskType, OpType, TMax> 
{
    using BitMaskBase<MaskType, OpType, TMax>::BitMaskBase; // Inherit constructors from BitMaskBase.
    BitMask(const BitMaskBase<MaskType, OpType, TMax>& other) : BitMaskBase<MaskType, OpType, TMax>(other) {}
};


// Specialized template for using enumerations as masks.
template <typename TEnum, typename MaskType = int>
struct Enummask : BitMaskBase<MaskType, TEnum, static_cast<int>(TEnum::MAX)>
{
    using BitMaskBase<MaskType, TEnum, static_cast<int>(TEnum::MAX)>::BitMaskBase; // Inherit constructors from BitMask.
    Enummask(const BitMaskBase<MaskType, TEnum, static_cast<int>(TEnum::MAX)>& other)
        : BitMaskBase<MaskType, TEnum, static_cast<int>(TEnum::MAX)>(other) {}
};

enum class MyEnum {
    Value1,
    Value2,
    Value3,
    Value4,
    MAX
};

int main() {
    BitMask<uint8_t> bitmask; // Create a BitMask with 8 bits, initialized to 0.

    // Set individual bits.
    bitmask.SetBit(2);  // Sets the 2nd bit to 1.
    bitmask.SetBit(5);  // Sets the 5th bit to 1.

    //#todo_khaled can we check for overflow dynamically
    std::cout << "BitMask 1: " << bitmask.toBinaryString() << std::endl;

    //0b1010 will be converted to Typemask

    // Using variadic template constructor.
    BitMask<uint16_t> bitmask2(1,3);  // Create a BitMask with 16 bits, sets bits 1 and 3.
    std::cout << "BitMask 2: " << bitmask2.toBinaryString() << std::endl;

    bool isBitSet = bitmask.IsBitSet(2);  // Checks if the 2nd bit is set.
    std::cout << "Is Bit 2 set in BitMask 1? " << std::boolalpha << isBitSet << std::endl;

    BitMask<uint8_t> bitmask1(3,0);
    BitMask<uint8_t> bitmask3(1,2);

    // Bitwise OR operation.
    BitMask<uint8_t> combined = bitmask1 + bitmask3;
    std::cout << "Combined BitMask: " << combined.toBinaryString() << std::endl;

    bitmask.ClearBit(2);  // Clears the 2nd bit (sets it to 0).
    std::cout << "Cleared BitMask 1: " << bitmask.toBinaryString() << std::endl;

    bool anyBitSet = bitmask.AnyBitSet();  // Checks if any bit is set.
    std::cout << "Any bit set in BitMask 1? " << std::boolalpha << anyBitSet << std::endl;

    // Enummask usage
    Enummask<MyEnum, uint16_t> enumMask; // Create an Enummask with 16 bits.

    enumMask.SetBit(MyEnum::Value1);  // Sets the bit corresponding to MyEnum::Value1.
    enumMask.SetBit(MyEnum::Value2);  // Sets the bit corresponding to MyEnum::Value2.

    std::cout << "Enummask: " << enumMask.toBinaryString() << std::endl;

    bool isValue1Set = enumMask.IsBitSet(MyEnum::Value1);  // Checks if the bit for MyEnum::Value1 is set.
    std::cout << "Is Value1 set in Enummask? " << std::boolalpha << isValue1Set << std::endl;

    bool anyBitSet2 = enumMask.AnyBitSet();  // Checks if any bit is set.
    std::cout << "Any bit set in Enummask? " << std::boolalpha << anyBitSet2 << std::endl;

    enumMask.ClearBit(MyEnum::Value2);  // Clears the bit corresponding to MyEnum::Value2.
    std::cout << "Cleared Value2 bit in Enummask: " << enumMask.toBinaryString() << std::endl;

    // Using variadic constructor for BitMask
    BitMask<uint8_t> bitmask4(2, 5);  // Sets bits 2 and 5.
    BitMask<uint16_t> bitmask5(1, 3); // Sets bits 1 and 3.

    std::cout << "BitMask 4: " << bitmask4.toBinaryString() << std::endl;
    std::cout << "BitMask 5: " << bitmask5.toBinaryString() << std::endl;

    // Using variadic constructor for Enummask
    Enummask<MyEnum, uint16_t> enumMaskVar(MyEnum::Value1, MyEnum::Value2); // Sets bits for Value1 and Value2.
    enumMaskVar.SetBit(MyEnum::Value3);
    std::cout << "enumMaskVar: " << enumMaskVar.toBinaryString() << std::endl;

    //#todo_Khaled only way I found to call varadic without unpacking in Enummask is to just call BitMask directly
    //using BitMaskBase<MaskType, TEnum, static_cast<int>(TEnum::MAX)>::BitMaskBase; vs EnumMask(const Args&... bits) : BitMask<TypeMask, static_cast<int>(TEnum::MAX)>(bits...) {}
    //#todo_Khaled I should assert the optype is of integral type

    // Using variadic constructor for BitMask directly
    BitMask<uint16_t> bitmaskVarint(MyEnum::Value1, MyEnum::Value2); // Sets bits for Value1 and Value2.
    std::cout << "bitmaskVarint: " << bitmaskVarint.toBinaryString() << std::endl;

    return 0;
}