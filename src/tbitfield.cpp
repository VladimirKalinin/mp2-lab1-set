// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdlib.h>
#include <iostream>

TBitField::TBitField(int len)
{   
	if (len>=0)
	{
		BitLen=len;
		MemLen=(len+31) >> 5;
		pMem= new TELEM [MemLen];
		for (int i=0; i<MemLen; i++)
			pMem[i]=0;
	}
	else throw exception();
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	pMem= new TELEM [MemLen];
	for (int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
}

TBitField::~TBitField()
{
	BitLen=0;
	MemLen=0;
	delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n/32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (1<<(n%32));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n>=0) && (n<=BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	else
		throw exception();
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n>=0) && (n<=BitLen))
		pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
	else
		throw exception();
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n>=0) && (n<=BitLen))
		if ((pMem[GetMemIndex(n)] & GetMemMask(n))==0)
			return 0;
		else
			return 1;
	else
		throw exception();
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	delete [] pMem;
	pMem= new TELEM [MemLen];
	for (int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int fl=1, i=0;
	if (BitLen!=bf.BitLen)
		return 0;
	else
	{
		while ((i<MemLen) && (fl !=0))
			{
				if (pMem[i]!=bf.pMem[i])
					fl=0;
				i++;
			}
		if (fl==1)
			return 1;
		else
			return 0;
	}

}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (*this==bf)
		return 0;
	else
		return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField tmp(max(BitLen, bf.BitLen));
	if (BitLen<bf.BitLen)
		{
			for (int i=0; i<MemLen; i++)
				tmp.pMem[i]=pMem[i];
			for (int i=MemLen; i<bf.MemLen; i++)
				tmp.pMem=0;
			for (int i=0; i<bf.MemLen; i++)
				tmp.pMem[i] |= bf.pMem[i];
		}
	if (BitLen>bf.BitLen)
		{
			for (int i=0; i<bf.MemLen; i++)
				tmp.pMem[i]=bf.pMem[i];
			for (int i=bf.MemLen; i<MemLen; i++)
				tmp.pMem=0;
			for (int i=0; i<MemLen; i++)
				tmp.pMem[i] |= pMem[i];
		}
	if (BitLen==bf.BitLen)
		{
			for (int i=0; i<MemLen; i++)
				tmp.pMem[i] = pMem[i] | bf.pMem[i];
		}
	return tmp;
}		



TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField tmp(max(BitLen, bf.BitLen));
	if (BitLen<bf.BitLen)
		{
			for (int i=0; i<MemLen; i++)
				tmp.pMem[i]=pMem[i];
			for (int i=MemLen; i<bf.MemLen; i++)
				tmp.pMem=0;
			for (int i=0; i<bf.MemLen; i++)
				tmp.pMem[i] &= bf.pMem[i];
		}
	if (BitLen>bf.BitLen)
		{
			for (int i=0; i<bf.MemLen; i++)
				tmp.pMem[i]=bf.pMem[i];
			for (int i=bf.MemLen; i<MemLen; i++)
				tmp.pMem=0;
			for (int i=0; i<MemLen; i++)
				tmp.pMem[i] &= pMem[i];
		}
	if (BitLen==bf.BitLen)
		{
			for (int i=0; i<MemLen; i++)
				tmp.pMem[i] = pMem[i] & bf.pMem[i];
		}
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i=0; i<BitLen;i++)
	{
		if (GetBit(i)==0)
			tmp.SetBit(i);
		else
			tmp.ClrBit(i);
	}
	return tmp;
}

// ввод/вывод

 istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i=0;
	char* tmp;
	istr >> tmp;
	while(tmp[i]!=0)
		i++;
	bf.~TBitField();
	bf = *(new TBitField(i));
	for (int j=0; j<i;j++)
		if (tmp[j]=='1')
			bf.SetBit(j);
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i=0; i < bf.BitLen; i++)
		if (bf.GetBit(i))
			ostr << 1;
		else
			ostr << 0;
	return ostr;
}