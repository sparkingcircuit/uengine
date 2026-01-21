bool isbitof32(uint8_t bit, int32_t value)
{
	return((value >> bit) & 0x0000000000000001);
}

bool isbitof8(uint8_t bit, uint32_t value)
{
	return((value >> bit) & 0x01);
}
