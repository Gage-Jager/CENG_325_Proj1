#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

/* ****************************************************************************
*   Function Prototypes
**************************************************************************** */

int get_num_data_bits();

int get_num_parity_bits(int num_data_bits);

int* generate_data_bits(int num_data_bits);

int** generate_encode_array_G(int num_total_bits, int num_data_bits);

int** generate_parity_check_array_H(int num_parity_bits, int num_total_bits);

int** generate_decode_array_R(int num_data_bits, int num_total_bits);

int main()
{


    srand(time(NULL));      //For generating the data bits later.

    return 0;
}

int get_num_data_bits()
{
    int num_data_bits = 0;

    cout << setw(50) << left << "Enter the number of data bits to be transmitted:";
    cin >> num_data_bits;       //Simply ask for the number of data bits.

    return num_data_bits;
}

int get_num_parity_bits(int num_data_bits)
{
    int num_parity_bits = 2;

    /* ************************************************************************
    * For n parity bits, we can have up to 2^n - n - 1  data bits.
    * We need 2 parity bits to have a single data bit, so start at 2.
    * Increment the number of parity bits until the amount of data bits the
    * parity count can handle at maximum is more than or equal to the amount
    * of data bits that we have.
    ************************************************************************ */

    while( ( static_cast<int>( pow(2, num_parity_bits) ) - num_parity_bits - 1 ) < num_data_bits )
    {
        num_parity_bits += 1;
    }

    return num_parity_bits;
}

int* generate_data_bits(int num_data_bits)
{
    int* data_bit_array = 0;
    data_bit_array = new int[num_data_bits];

    for( int i = 0; i < num_data_bits; i++ )
    {
        data_bit_array[i] = rand() % 2;     //Generate either 0 or 1 for every data bit.
    }

    return data_bit_array;
}

int** generate_encode_array_G(int num_total_bits, int num_data_bits)
{
    int** encode_array = 0;

    return encode_array;
}

int** generate_parity_check_array_H(int num_parity_bits, int num_total_bits)
{
    int** parity_check_array = 0;

    return parity_check_array;
}

int** generate_decode_array_R(int num_data_bits, int num_total_bits)
{
    int** decode_array = 0;

    return decode_array;
}
