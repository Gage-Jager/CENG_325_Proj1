//To compile, please enter the following into a terminal opened to the directory this file is in.
//cl /EHsc CENG_325_Proj1.cpp

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

/* ****************************************************************************
*   Function Prototypes
**************************************************************************** */

bool is_power_of_two(int num_to_check);

int get_num_data_bits();

int get_num_parity_bits(int num_data_bits);

int* generate_data_bits_array_p(int num_data_bits);

int** generate_encode_array_G(int num_total_bits, int num_data_bits);

int** generate_parity_check_array_H(int num_parity_bits, int num_total_bits);

int** generate_decode_array_R(int num_data_bits, int num_total_bits);

int* encode_data_bits_p(int**& encode_array_G, int*& data_bits_p, int num_total_bits, int num_data_bits);

void induce_error_in_x(int*& encoded_data_x, int num_total_bits);

int* parity_check(int**& parity_check_array_H, int*& encoded_data_x, int num_parity_bits, int num_total_bits);

void fix_error(int*& syndrome_z, int*& encoded_data_x, int num_parity_bits);

int* decode_data_bits_p(int**& decode_array_R, int*& encoded_data_x, int num_data_bits, int num_total_bits);

/* ****************************************************************************
*   Function Definitions
**************************************************************************** */

int main()
{
    int i = 0;
    int num_data_bits = 0;
    int num_parity_bits = 0;
    int num_total_bits = 0;
    int* data_bits_p = 0;
    int** encode_array_G = 0;
    int** parity_check_array_H = 0;
    int** decode_array_R = 0;
    int* encoded_data_x = 0;
    int* syndrome_z = 0;
    int* decoded_data_bits_p = 0;

    srand( static_cast<unsigned int>(time(NULL)) );      //For generating the data bits later.

    //Prompt the user for the number of data bits.
    num_data_bits = get_num_data_bits();

    //Ensure the user entered a number greater than one, as specified by the "For an A" requirements.
    if( num_data_bits <= 1 )
    {
        cout << "Please enter a number greater than 1." << endl;

        return -1;
    }

    //Generate the number of parity bits, then caculate the total number of bits.
    num_parity_bits = get_num_parity_bits(num_data_bits);
    num_total_bits = num_data_bits + num_parity_bits;

    //Randomly generate some data bits.
    data_bits_p = generate_data_bits_array_p(num_data_bits);

    //Show the user the generated message.
    cout << setw(28) << left << "Your Message Is" << ": [";

    for( int i = 0; i < (num_data_bits - 1); i++ )
    {
        cout << data_bits_p[i] << ' ';
    }

    cout << data_bits_p[num_data_bits - 1] << ']' << endl;

    //Generate the encoding array, G, of 'total bits' rows and 'data bits' columns.
    encode_array_G = generate_encode_array_G(num_total_bits, num_data_bits);

    //Generate the parity check array, H, of 'parity bits' rows and 'total bits' columns.
    parity_check_array_H = generate_parity_check_array_H(num_parity_bits, num_total_bits);

    //Generate the decoding array, H, of 'data bits' rows and 'total bits' columns.
    decode_array_R = generate_decode_array_R(num_data_bits, num_total_bits);

    //Encode the data bits to get the Outgoing Message.
    encoded_data_x = encode_data_bits_p(encode_array_G, data_bits_p, num_total_bits, num_data_bits);

    //Show the user the encoded message.
    cout << setw(28) << left << "Encoded Message Is" << ": [";

    for( int i = 0; i < (num_total_bits - 1); i++ )
    {
        cout << encoded_data_x[i] << ' ';
    }

    cout << encoded_data_x[num_total_bits - 1] << ']' << endl;

    //Induce an random error in the encoded message.
    induce_error_in_x(encoded_data_x, num_total_bits);

    //Show the user the recieved message.
    cout << setw(28) << left << "Recieved Message Is" << ": [";

    for( int i = 0; i < (num_total_bits - 1); i++ )
    {
        cout << encoded_data_x[i] << ' ';
    }

    cout << encoded_data_x[num_total_bits - 1] << ']' << endl;

    //Calculate the syndrome array to find if there was an error.
    syndrome_z = parity_check(parity_check_array_H, encoded_data_x, num_parity_bits, num_total_bits);

    //Output the syndrome for the user.
    cout << setw(28) << left << "Parity Check (Little Endian)" << ": [";

    for( int i = 0; i < (num_parity_bits - 1); i++ )
    {
        cout << syndrome_z[i] << ' ';
    }

    cout << syndrome_z[num_parity_bits - 1] << ']' << endl;

    //Determine if there was an error, and fix it if there was.
    i = 0;

    while( (syndrome_z[i] == 0) && (i < num_parity_bits) )
    {
        i += 1;
    }

    if( i != num_parity_bits )
    {
        //There was an error.
        fix_error(syndrome_z, encoded_data_x, num_parity_bits);
    }

    //Output the corrected message for the user.
    cout << setw(28) << left << "Corrected Message Is" << ": [";

    for( int i = 0; i < (num_total_bits - 1); i++ )
    {
        cout << encoded_data_x[i] << ' ';
    }

    cout << encoded_data_x[num_total_bits - 1] << ']' << endl;

    //Decode the message.
    decoded_data_bits_p = decode_data_bits_p(decode_array_R, encoded_data_x, num_data_bits, num_total_bits);

    //Output the decoded message for the user.
    cout << setw(28) << left << "Decoded Message Is" << ": [";

    for( int i = 0; i < (num_data_bits - 1); i++ )
    {
        cout << decoded_data_bits_p[i] << ' ';
    }

    cout << decoded_data_bits_p[num_data_bits - 1] << ']' << endl;

    //Free up any dynamically allocated variables from main.

    delete[] data_bits_p;

    for( int i = 0; i < num_total_bits; i++ )
    {
        delete[] encode_array_G[i];
    }
    delete[] encode_array_G;

    for( int i = 0; i < num_parity_bits; i++ )
    {
        delete[] parity_check_array_H[i];
    }
    delete[] parity_check_array_H;

    for( int i = 0; i < num_data_bits; i++ )
    {
        delete[] decode_array_R[i];
    }
    delete[] decode_array_R;

    delete[] encoded_data_x;

    delete[] syndrome_z;
    
    delete[] decoded_data_bits_p;

    //End scene.
    return 0;
}

bool is_power_of_two(int num)
{
    //A number is a power of two if the bitwise AND of it and itself minus 1 is 0.
    //We also only want the positve powers, 1, 2, 4, 8, etc.

    //eg. num = 4.  4 = 100.  4-1 = 3 = 011.  100 AND 011 = 000. Power of 2.
    //eg. num = 5.  5 = 101.  5-1 = 4 = 100.  101 AND 100 = 100. Not Power of 2.
    return (num > 0) && ((num & (num - 1)) == 0);
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

int* generate_data_bits_array_p(int num_data_bits)
{
    int* data_bits_p = 0;
    data_bits_p = new int[num_data_bits];

    for( int i = 0; i < num_data_bits; i++ )
    {
        data_bits_p[i] = rand() % 2;     //Generate either 0 or 1 for every data bit.
    }

    return data_bits_p;
}

int** generate_encode_array_G(int num_total_bits, int num_data_bits)
{
    int** encode_array_G = 0;
    int identity_counter = 0;
    int parity_counter = 0;
    int bit_number = 0;
    int k = 0;

    //Create the array with the proper dimensions.

    encode_array_G = new int* [num_total_bits];
    for( int i = 0; i < num_total_bits; i++ )
    {
        encode_array_G[i] = new int [num_data_bits];
    }

    //Initialize with zeroes.

    for( int i = 0; i < num_total_bits; i++ )
    {
        for( int j = 0; j < num_data_bits; j++ )
        {
            encode_array_G[i][j] = 0;
        }
    }

    //Fill in the values with ones in the correct locations.

    for( int i = 0; i < num_total_bits; i++ )
    {
        
        
        //For every row, check if it corresponds to a parity bit (power of 2).
        //If it is, put a one in the columns corresponding to the data bits covered by that parity bit.
        //If it isn't, put a row from the identity matrix in instead.

        if( is_power_of_two(i + 1) )
        {
            k = 1;

            for (int j = 0; j < num_data_bits; j++)
            {
                //For every column in row i, get the location of a data bit in a hypothetical encoded string.
                //j=0, k=3; j=1, k=5; j=2, k=6; etc.

                while( is_power_of_two(k) )
                {
                    k += 1;
                }

                bit_number = k;
                k += 1;

                //The first parity bit covers data bits in positions xxx1, the second xx1x, and so on.
                //To find if a data bit is covered by a certain parity bit, shift the data bit location right by
                //the number of parity bits completed, and then bitwise AND with 1 to find if we need to place a 1 in G.

                for(int m = 0; m < parity_counter; m++)
                {
                    bit_number = bit_number >> 1;
                }

                if( (bit_number & 1) == 1)
                {
                    encode_array_G[i][j] = 1;
                }
                
            }

            parity_counter += 1;
        }
        else
        {
            encode_array_G[i][identity_counter] = 1;
            identity_counter += 1;
        }
    }

    return encode_array_G;
}

int** generate_parity_check_array_H(int num_parity_bits, int num_total_bits)
{
    int** parity_check_array_H = 0;
    int bit_number = 0;
    int shift_number = 0;

    //Create the array with proper dimensions.

    parity_check_array_H = new int* [num_parity_bits];

    for( int i = 0; i < num_parity_bits; i++ )
    {
        parity_check_array_H[i] = new int[num_total_bits];
    }

    //Initialize with zeroes.

    for( int i = 0; i < num_parity_bits; i++ )
    {
        for( int j = 0; j < num_total_bits; j++ )
        {
            parity_check_array_H[i][j] = 0;
        }
    }

    /*
    * Each row in H corresponds to a parity bit, and the columns correspond to
    * which bits are covered by a parity bit.
    * First row  = 1 0 1 0 1 0 1....
    * Second row = 0 1 1 0 0 1 1....
    * Third row  = 0 0 0 1 1 1 1....
    * and so on.
    * 
    * We can use the same logic from generating G,
    * but instead of ignoring parity bit locations,
    * we include all bits.
    */

    for (int i = 0; i < num_parity_bits; i++)
    {
        bit_number = 1;

        for (int j = 0; j < num_total_bits; j++)
        {
            shift_number = bit_number;

            //The first parity bit covers data bits in positions xxx1, the second xx1x, and so on.
            //To find if a data bit is covered by a certain parity bit, shift the data bit location right by
            //the number of parity bits (rows) completed, and then bitwise AND with 1 to find if we need to place a 1 in H.

            for (int m = 0; m < i; m++)
            {
                shift_number = shift_number >> 1;
            }

            if ((shift_number & 1) == 1)
            {
                parity_check_array_H[i][j] = 1;
            }

            bit_number += 1;
        }
    }

    return parity_check_array_H;
}

int** generate_decode_array_R(int num_data_bits, int num_total_bits)
{
    int** decode_array_R = 0;
    int curr_row_for_identity = 0;

    //Create the array with proper dimensions.
    decode_array_R = new int* [num_data_bits];

    for( int i = 0; i < num_data_bits; i++ )
    {
        decode_array_R[i] = new int[num_total_bits];
    }

    //Initialize with zeros.
    for( int i = 0; i < num_data_bits; i++ )
    {
        for( int j = 0; j < num_total_bits; j++ )
        {
            decode_array_R[i][j] = 0;
        }
    }

    //Create the identity matrix in the columns that are not powers of two.
    for( int i = 0; i < num_total_bits; i++ )
    {
        if( !( is_power_of_two(i + 1) ) )
        {
            decode_array_R[curr_row_for_identity][i] = 1;
            
            curr_row_for_identity += 1;
        }
    }

    return decode_array_R;
}

int* encode_data_bits_p(int**& encode_array_G, int*& data_bits_p, int num_total_bits, int num_data_bits)
{
    int* encoded_data_x = 0;
    int sum = 0;

    //Create the array of proper size.
    encoded_data_x = new int[num_total_bits];

    //Multiply G by p to get the initial x.
    for( int i = 0; i < num_total_bits; i++ )
    {
        sum = 0;

        for( int j = 0; j < num_data_bits; j++ )
        {
            sum += ( encode_array_G[i][j] * data_bits_p[j] );
        }

        encoded_data_x[i] = sum;
    }

    //Initial x modulus 2 gives us x in only 1s and 0s.
    for( int i = 0; i < num_total_bits; i++ )
    {
        encoded_data_x[i] = encoded_data_x[i] % 2;
    }

    return encoded_data_x;
}

void induce_error_in_x(int*& encoded_data_x, int num_total_bits)
{
    int random_bit = 0;
    int read_bit = 0;

    //Pick a random bit from 0 to num_total_bits + 3, and flip that bit, if it exists.
    //For bits num_total_bits to num_total_bits + 3, there will be no error.

    random_bit = rand() % (num_total_bits + 4);

    if( random_bit < num_total_bits)
    {
        read_bit = encoded_data_x[random_bit];

        if( read_bit == 0 )
        {
            encoded_data_x[random_bit] = 1;
        }
        else
        {
            encoded_data_x[random_bit] = 0;
        }
    }

    return;
}

int* parity_check(int**& parity_check_array_H, int*& encoded_data_x, int num_parity_bits, int num_total_bits)
{
    int* syndrome_z = 0;
    int sum = 0;

    //Give it the proper size.
    syndrome_z = new int[num_parity_bits];

    //Initialize with zeroes;
    for( int i = 0; i < num_parity_bits; i++ )
    {
        syndrome_z[i] = 0;
    }

    //Multiply H and x to recieve initial z.
    for( int i = 0; i < num_parity_bits; i++ )
    {
        sum = 0;

        for( int j = 0; j < num_total_bits; j++ )
        {
            sum += parity_check_array_H[i][j] * encoded_data_x[j];
        }

        syndrome_z[i] = sum;
    }

    //Performe initial z modulus 2 to get the syndrome z.
    for( int i = 0; i < num_parity_bits; i++ )
    {
        syndrome_z[i] = syndrome_z[i] % 2;
    }

    return syndrome_z;
}

void fix_error(int*& syndrome_z, int*& encoded_data_x, int num_parity_bits)
{
    int error_position = 0;
    int read_bit = 0;

    //Obtain the location of the error from syndrome z.
    for( int i = (num_parity_bits - 1); i >= 0; i--)
    {
        error_position = error_position << 1;
        error_position = error_position | syndrome_z[i];
    }

    //Flip the bit in the specified location.
    read_bit = encoded_data_x[error_position - 1];

    if (read_bit == 0)
    {
        encoded_data_x[error_position - 1] = 1;
    }
    else
    {
        encoded_data_x[error_position - 1] = 0;
    }

    return;
}

int* decode_data_bits_p(int**& decode_array_R, int*& encoded_data_x, int num_data_bits, int num_total_bits)
{
    int* decoded_data_bits_p = 0;
    int sum = 0;

    //Generate the correct size.
    decoded_data_bits_p = new int[num_data_bits];

    //Initialize with zeroes.
    for( int i = 0; i < num_data_bits; i++ )
    {
        decoded_data_bits_p[i] = 0;
    }

    //Multiply R by x and modulus by 2 to return to p, the decoded message.
    for( int i = 0; i < num_data_bits; i++ )
    {
        sum = 0;

        for( int j = 0; j < num_total_bits; j++ )
        {
            sum += decode_array_R[i][j] * encoded_data_x[j];
        }

        decoded_data_bits_p[i] = sum;
    }

    for( int i = 0; i < num_data_bits; i++ )
    {
        decoded_data_bits_p[i] = decoded_data_bits_p[i] % 2;
    }

    return decoded_data_bits_p;
}