#include <stdio.h>
#include <mpi.h>
#include <time.h>

#define TAG1 1
#define TAG2 2

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

// define processor inputs and outputs, processors receives two ints, sorts them and sends
void cmp_swap(int in1, int in2, int out1, int out2, int tag1, int tag2)
{ 
    MPI_Status status;

    int in1_val;
    int in2_val;

    MPI_Recv(&in1_val,1,MPI_INT,in1,TAG1, MPI_COMM_WORLD,&status);
    MPI_Recv(&in2_val,1,MPI_INT,in2,TAG2, MPI_COMM_WORLD,&status);

    int min_val = min(in1_val,in2_val);
    int max_val = max(in1_val,in2_val);

    MPI_Send(&min_val,1,MPI_INT,out1,tag1, MPI_COMM_WORLD);
    MPI_Send(&max_val,1,MPI_INT,out2,tag2, MPI_COMM_WORLD);
}

// loads 8 chars from file called "numbers"
void load_data(int *input_data)
{
    // open file
    FILE *fptr = fopen("numbers","r");
    if(fptr == NULL)
    {
        fprintf(stderr,"Cant open file called \"numbers\"\n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    // store numbers
    unsigned int c;
    int pos = 0;

    while( (c = getc(fptr)) != EOF)
    {
        input_data[pos] = c;
        pos++;
    }

    // print numbers
    for( int i = 0; i < 8; i++)
    {
        if( i < 7)
        {
            printf("%d ",input_data[i]);
        }else
        {
            printf("%d\n",input_data[i]);
        }
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // processor 0 loads data and sends them to other processors
    if(rank == 0 )
    {
        int input_data[] = {0,0,0,0,0,0,0,0};
        load_data(input_data);

        MPI_Send(input_data,1,MPI_INT,0,TAG1, MPI_COMM_WORLD);
        MPI_Send(input_data+1,1,MPI_INT,0,TAG2, MPI_COMM_WORLD);
        MPI_Send(input_data+2,1,MPI_INT,1,TAG1, MPI_COMM_WORLD);
        MPI_Send(input_data+3,1,MPI_INT,1,TAG2, MPI_COMM_WORLD);
        MPI_Send(input_data+4,1,MPI_INT,2,TAG1, MPI_COMM_WORLD);
        MPI_Send(input_data+5,1,MPI_INT,2,TAG2, MPI_COMM_WORLD);
        MPI_Send(input_data+6,1,MPI_INT,3,TAG1, MPI_COMM_WORLD);
        MPI_Send(input_data+7,1,MPI_INT,3,TAG2, MPI_COMM_WORLD);

    }

    //processors work as compare and swap, send, sort, receive numbers
    switch(rank)
    {
    case 0:

        cmp_swap(0,0,4,5,TAG1,TAG1);
        break;

    case 1:

        cmp_swap(0,0,4,5,TAG2,TAG2);
        break;

    case 2:

        cmp_swap(0,0,6,7,TAG1,TAG1);
        break;

    case 3:

        cmp_swap(0,0,6,7,TAG2,TAG2);
        break;

    case 4:

        cmp_swap(0,1,10,8,TAG1,TAG1);
        break;

    case 5:

        cmp_swap(0,1,8,13,TAG2,TAG1);
        break;

    case 6:

        cmp_swap(2,3,10,9,TAG2,TAG1);
        break;

    case 7:

        cmp_swap(2,3,9,13,TAG2,TAG2);
        break;

    case 8:

        cmp_swap(4,5,12,11,TAG1,TAG1);
        break;

    case 9:

        cmp_swap(6,7,12,11,TAG2,TAG2);
        break;

    case 10:

        cmp_swap(4,6,0,14,TAG1,TAG1);
        break;

    case 11:

        cmp_swap(8,9,14,18,TAG2,TAG1);
        break;

    case 12:

        cmp_swap(8,9,16,15,TAG2,TAG1);
        break;

    case 13:

        cmp_swap(5,7,15,0,TAG2,TAG1);
        break;

    case 14:

        cmp_swap(10,11,16,17,TAG1,TAG1);
        break;

    case 15:

        cmp_swap(12,13,17,18,TAG2,TAG2);
        break;

    case 16:

        cmp_swap(14,12,0,0,TAG1,TAG2);
        break;

    case 17:

        cmp_swap(14,15,0,0,TAG1,TAG2);
        break;

    case 18:
        
        cmp_swap(11,15,0,0,TAG1,TAG2);
        break;
    
    default:
        break;
    }

    //processor 0 receives sorted numbers and prints them
    if(rank == 0)
    {
        int output_data[] = {0,0,0,0,0,0,0,0};
        MPI_Status status;
        MPI_Recv(output_data,1,MPI_INT,10,TAG1, MPI_COMM_WORLD,&status);
        MPI_Recv(output_data+1,1,MPI_INT,16,TAG1, MPI_COMM_WORLD,&status);
        MPI_Recv(output_data+2,1,MPI_INT,16,TAG2, MPI_COMM_WORLD,&status);
        MPI_Recv(output_data+3,1,MPI_INT,17,TAG1, MPI_COMM_WORLD,&status);
        MPI_Recv(output_data+4,1,MPI_INT,17,TAG2, MPI_COMM_WORLD,&status);
        MPI_Recv(output_data+5,1,MPI_INT,18,TAG1, MPI_COMM_WORLD,&status);
        MPI_Recv(output_data+6,1,MPI_INT,18,TAG2, MPI_COMM_WORLD,&status);
        MPI_Recv(output_data+7,1,MPI_INT,13,TAG1, MPI_COMM_WORLD,&status);

        for(int i = 0; i < 8; i++)
        {
            printf("%d\n",output_data[i]);
        }
    }

    MPI_Finalize();
}
