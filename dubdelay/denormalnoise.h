class denormalnoise
{
public:
        denormalnoise(int size)
        {
                rand_state = 1;
                noisetbl = new float[size];
                for (int i = 0; i<size;i++){
                        noisetbl[i] = white_noise();
                }
        }
        denormalnoise()
        {
                rand_state = 1;
                noisetbl = new float[257];
                for (int i = 0; i<257;i++){
                        noisetbl[i] = white_noise();
                }
        }
        ~denormalnoise()
        {
                delete [] noisetbl;
        }
private:
        unsigned int rand_state;
        float * noisetbl;

        float white_noise ()
        {
                rand_state = rand_state * 1234567UL + 890123UL;
                int mantissa = rand_state & 0x807F0000; // Keep only most significant bits
                int flt_rnd = mantissa | 0x1E000000; // Set exponent
                return *reinterpret_cast <const float *> (&flt_rnd);
        }

public:
        inline float get(const int p)
        {
                return noisetbl[p];
        }

}; 