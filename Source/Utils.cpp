/*
 * C++ will return negative modulo if a is negative.
 */
int mod(int a, int b)
{
   if(b < 0)
     return mod(-a, -b);
   int ret = a % b;
   if(ret < 0)
     ret+=b;
   return ret;
}
