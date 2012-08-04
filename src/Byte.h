#ifndef BYTE_H
#define BYTE_H

#define CHAR_EQUAL_TO_BYTE sizeof(char)==1

class Byte
{
public:
	Byte(unsigned char value=0):_cByte(value)
	{}
	void SwitchBits(int index1, int index2)
	{
		bool bit1=getBit(index1);
		setBit(index1,getBit(index2));
		setBit(index2,bit1);
	};
	unsigned char leastNibble()
	{
		return (_cByte&0x0F);
	}
	unsigned char mostNibble()
	{
		return (_cByte>>4);
	}
	
	bool getBit(int index)
	{
//#pragma warning(disable:4800)
		return _cByte&(1<<index)?true:false;
//#pragma warning(default:4800)
	}
	unsigned char getByte()
	{
		return _cByte;
	}
	void setBit(int index,bool state)
	{
		if (getBit(index)!=state)
		{
			toggle(index);
		}
		return;
	}
	unsigned char toggle(int index)
	{
		return (_cByte^=(1<<index));
	}
	operator unsigned char()
	{
		return _cByte;
	}
	bool operator[](int index)
	{
//#pragma warning(disable:4800)
		return _cByte&(1<<index)?true:false;
//#pragma warning(default:4800)
	}
	char operator~()
	{
		return ~(*this)._cByte;
	}
	void operator=(const char& cValue)
	{
		_cByte=cValue;
	}
private:
	unsigned char _cByte;
};



#endif