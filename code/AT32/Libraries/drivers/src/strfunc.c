#include<strfunc.h>

char *itoa(uint32_t num)
{
	//����һ�����飬��������ַ�
	//�Ѵ��ݹ�������ֵ����ʮȡȡ�࣬�ѵõ������ּӡ�0����Ȼ���ŵ�������
	//�����Ǹ��������
	//������鷴ת��ĩβ�ӡ�\0��
	//return str
	char str[64] = "";
	static char str1[64] ="hello";
	uint32_t i = 0,j = 0;
	
	for(i = 0;i < 64 && num >= 1;i++)
	{
		str[i]=num%10 + '0';
		num /= 10;
		
	}
	while(i>0)
	{
		str1[j++] = str[i - 1];
		i--;
	}
	str1[j] = ' ';
	str1[j+1] = ' ';
	str1[j+2] = '\0';

	return str1;
}
