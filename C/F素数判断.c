int prime(int input) // ����0��������������1������
{
    int i, output;
    if (input <= 1)
        output = 0;
    else
    {
        for (i = 2, output = 1; i < input; i++)
        {
            if (input % i == 0)
            {
                output = 0;
                break;
            }
        }
    }
    return output;
}