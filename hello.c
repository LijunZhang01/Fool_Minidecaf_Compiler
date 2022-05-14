int a=3;
int b;

int sub_3(int x, int y, int z) {
    return x - y - z;
}

int main() {
    int d=7;
    int b=sub_3(10, 4, 2);
    int c=a+b+d;
    return c;
}