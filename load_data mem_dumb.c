#include <stdio.h>
#include <assert.h>

#define MEMSIZE (64*1024)

typedef unsigned char byte;
typedef unsigned int word;
typedef word address;

byte mem[MEMSIZE];

void b_write (address adr, byte val)
{
    mem[adr] = val;
}
byte b_read (address adr)
{
    return mem[adr];
}

word w_read (address a)
{
    word w = mem[a+1] << 8;
    w = w | mem[a];
    return w & 0xFFFF;
}

void w_write(address a, word val)
{
    // младший байт
    mem[a] = (byte)val;
    mem[a+1] = (byte)(val >> 8);
}

void load_data(const char *filename)
{
    if (filename == NULL) {
        filename = DEFAULT_FILENAME;
    }
    FILE *f = fopen(filename, "r");
    //Hàm fopen() được sử dụng để mở tệp tin với tên được truyền vào trong chế độ đọc "r". 
    //Sau đó, assert() được sử dụng để kiểm tra xem tệp tin có được mở thành công
    assert(f != NULL);

    int adr, n;
    //Nếu fscanf() đọc được cả 2 giá trị adr và n và trả về giá trị bằng 2 thì điều kiện trong while là đúng và vòng lặp sẽ được thực thi, trong trường hợp ngược lại, khi fscanf() không đọc được cả 2 giá trị thì vòng lặp sẽ kết thúc.
    while (fscanf(f, "%x%x", &adr, &n) == 2)
    {   //Biến n là số byte được đọc từ tệp tin vào lần lượt ghi vào bộ nhớ. 
        for (int i = 0; i < n; i++)
        {
            int bVal;
            fscanf(f, "%x", &bVal);
            b_write(adr + i, bVal);
            //hàm fscanf được sử dụng để đọc một byte giá trị thập lục phân (hexadecimal) từ tệp tin vào biến bVal, 
            //sau đó gọi hàm b_write để ghi bVal vào vị trí bộ nhớ đang được quản lý bởi con trỏ adr
        }
    }

    fclose(f);//tệp tin được đóng lại bằng lệnh fclose(f).
}
//Hàm mem_dump được sử dụng để in ra các giá trị lưu trữ trong bộ nhớ, bắt đầu từ địa chỉ adr và có kích thước size byte.
void mem_dump(address adr, int size)
{
    for (int i = 0; i < size; i += 2)//ta muốn in ra dưới dạng từng từ (2 byte) nên ta sử dụng i += 2 để tăng giá trị i lên 2 mỗi lần lặp.
    {
        word w = w_read(adr + i);
        printf("%06o: %06o %04x\n", adr + i, w, w);
    }
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    load_data(argv[1]);
    mem_dump(0, MEMSIZE);

    return 0;
}