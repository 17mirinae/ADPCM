#include<stdio.h>
#define FINISH -9999999		// �����Ⱚ���� �Է°��� ���� ������.

/*IMA-ADPCM��ȣȭ �˰����� ���� ������̺�*/
const int indexTable[16]={				
	-1, -1, -1, -1, 2, 4, 6, 8,
	-1, -1, -1, -1, 2, 4, 6, 8
};
const int stepTable[89]={
	7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
	19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
	50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
	130, 143, 157, 173, 190, 209, 230, 253, 279,
	307, 337, 371, 408, 449, 494, 544, 598, 658,
	724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552,
	1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327,
	3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132,
	7845, 8630, 9493, 10442, 11487, 12635, 13899,
	15289, 16818, 18500, 20350, 22385,
	24623, 27086, 29794, 32767
};
/*encoder�� ���ι����� �����Ͽ���.*/
int Quantizer(int diff, int step);
int Dequantizer(int code, int step);
int main(void)
{
	int input[100], codeword[100]={0}, pred_sample, pred_diff, step, step_data, index, n, mask, code=0, diff=0, recon;
	int first_flag=1;
	for(n=0;n<100;n++) input[n] = FINISH;// �������� ���� �˼��ְ� �ʱ�ȭ���ش�./
	/*������ ���� ��� ����3-6�� ������ �ʱ�ȭ �Ͽ���.*/
	input[0] = 81;
	input[1] = 97;
	input[2] = 107;
	input[3] = 92;
	input[4] = 64;
	/*3-6�ʱⰪ*/
	n=0;
	while(input[n]!=FINISH){
		if(first_flag){		//ó�� �����ϴ°��� flag�� �̿�
			first_flag = 0;
			step = 50;
			index = 20;
			pred_sample = 0;
		}//����3-6�� �ʱⰪ.
		diff = input[n] - pred_sample;
		codeword[n] = Quantizer(diff, step);//����ȭ
		pred_diff = Dequantizer(&codeword[n], step);//������ȭ
		if(diff<0) pred_diff = -pred_diff;//msbó��/
		pred_sample = pred_sample + pred_diff;
		printf("���� : %d  �Է�ǥ�� : %d  ���̰� : %d  �ڵ���� : %d\n", n+1, input[n], diff, codeword[n]);//�ڵ���� ����� int������ ��.
		printf("���� ���̰� : %d,  ���� ǥ�� : %d,  ����ũ�� : %d\n\n",pred_diff, pred_sample, step);
		index = index + indexTable[codeword[n]];
		step = stepTable[index];
		n++;
	}
	printf("���� ���� ũ�� : %d\n", step);
	return 0;
}
int Quantizer(int diff, int step){
	int code, mask, n;
	if(diff<0){ diff = -diff, code=0x0008;}
	else code = 0;
	mask = 4;
	for(n=1;n<=3;n++){
		if(diff>=step){
			code |= mask;
			diff = diff - step;
		}
		step = step>>1;
		mask = mask>>1;
	}
	return code;
}
int Dequantizer(int *code_data, int step){
	int recon, mask, n, code=*code_data;
	recon = 0;
	mask = 4;
	code = code | 0x0000;
	for(n=1;n<=3;n++){
		if(code&mask) recon = recon + step;
		step = step>>1;
		mask=mask>>1;
	}
	recon = recon + step;
	if(code&0x0008) code=-code;

	*code_data = code;
	return recon;
}