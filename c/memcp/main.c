char* memcpy(char* dest, const char* source, int length)
{
    char* ret = dest;
    //  判是否为空
    if (0 == source || 0 == dest) {
	return dest;
    }
    while (length) {
	*dest = *source;
	source++;
	dest++;
	length--;
    }
    return ret;
}

int main()
{
    char a[10] = "123";
    char b[10] = "345678";
    memcpy(a, b, 7);
    printf("%s", a);
}
