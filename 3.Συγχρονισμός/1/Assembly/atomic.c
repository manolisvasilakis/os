void main()
{
	int val = 0;
	__sync_fetch_and_add(&val, 1);
}
