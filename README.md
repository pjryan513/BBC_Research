# BBC_Research

The purpose of this research project was to improve upon the Byte-aligned Bitmap Code (BBC) compression algorithm.
This new variation of the BBC algorithm is called the Byte-algined Bitmap Exponetial Compression (BBEC) algorithm.  

## How BBC works

BBC is a fill-based compression algorithm that is capable of storing both 0 and 1 fills.  
The algorithm compresses raw data into portions called runs, there are four kinds of runs in BBC.

## How BBEC works

The Bit-aligned Bitmap Exponentail Compression algorithm is the same as the BBC algorithm expect it adds a fifth run-type.  The fifth run type is used to store fills (long sequence of 0s or 1s) in an expetional format.  The header of the fifth run type looks as follows, [0 0 0 0 | fill_bit(1 bit) | linear follow (1 bit) | tail (2 bits) ].  These header bytes are similar to past header bytes expect the tail is only two bytes and a new type of bit is added, the linear follow.  The linear follow bit states whether there is a linear store byte that comes at the end of the sequnce, 1 means there is a linear store byte and 0 means there is not. After the header comes the exponetial byte which looks like [ follower bit (1 bit) | base (3 bits) | exponent (4 bits) ].  The follwer bit tells whether another exponetial byte will follow this byte, 1 means there is another byte following, and 0 means there is not.  The base bits represents the base number and the exponent bits represents the exponent.  For both the the base and the exponent the numbers stored represent two more than what they are.  For example if base is 0 and exponent is 3 then the base is really 2 and the exponent is really 5.  This is because a base and exponent of either 0 or 1 is not of any use.  The linear follow will only be used if there is 255 or fewer fill bytes remain after exponetial bytes are used.  This is if the number of remaining fills are below 127 then we can just use a single byte to store the reamining fills.

### Exponetial Decomposition
This term referes to the method used to store fills in an exponetial format.
