#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<graphics.h>
#include<conio.h>
#pragma warning(disable:4996)

#define  sel_agorithm  1

const int max_n = 500;

int r1[] = { 30,20,130,60 };
int r2[] = { 160,20,260,60 };
int r3[] = { 290,20,340,60 };


//蛮力法
int duel_force(int n,int matrix[max_n][max_n])
{
	int possible_winners = 0;

	
	for (int i = 0; i < n; i++)
	{
		int current = i;
		
		int is_winner = 1; 

		//每个人需要进行n-1场决斗
		for (int j = 0; j < n-1; j++)
		{
			int next = (current + 1) % n; 

			if (matrix[current][next] == 0)
			{
				is_winner = 0;
				break;
			}

			current = next;
		}

		if (is_winner)
		{
			possible_winners++;
		}
	}

	return possible_winners;
}


//动态规划
int duel_dynamic(int n,int matrix[max_n][max_n])
{
	int i, j, k;

	int possible_winners = 0;

	int** dp = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
	{
		dp[i] = (int*)malloc(n * sizeof(int));
		memcpy(dp[i], matrix[i], n * sizeof(int));
	}


	for (i = 1; i < n; i++)
	{
		for (int start = 0; start < n; start++)
		{
			int end = (start + i + 1) % n;
			if (dp[start][end])
				continue;
			for (j = (start + 1) % n; j != end; j++, j %= n)
			{
				if (dp[start][j] && dp[j][end] && (matrix[start][j] || matrix[end][j]))
				{
					dp[start][end] = true;
					break;
				}
			}
		}
	}

	for (i = 0; i < n; i++)
	{
		if (dp[i][i])
		{
			possible_winners++;
		}
	}

	for (i = 0; i < n; i++)
	{
		free(dp[i]);
	}
	free(dp);

	return possible_winners;
}

int is_button(int x, int y)
{
	if (x > r1[0] && x<r1[2] && y>r1[1] && y < r1[3])
		return 1;

	if (x > r2[0] && x<r2[2] && y>r2[1] && y < r2[3])
		return 2;

	if (x > r3[0] && x<r3[2] && y>r3[1] && y < r3[3])
		return 3;

	return 0;
}

int main()
{
	TCHAR data[500];

	int N; //测试数据的组数

	int possible_winners = 0;

	int results[20];
	int num_result = 0;

	//initgraph(480, 360, EW_SHOWCONSOLE);
	initgraph(480, 360);

	//渐变背景色
	for (int i = 0; i < 256; i += 5)
	{
		setbkcolor(RGB(i, i, i));
		//setbkcolor(WHITE);
		cleardevice();
		Sleep(30);
	}

	RECT R1 = { r1[0],r1[1],r1[2],r1[3] };
	RECT R2 = { r2[0],r2[1],r2[2],r2[3] };
	RECT R3 = { r3[0],r3[1],r3[2],r3[3] };

	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY; //设置输出效果
	settextcolor(BLACK);
	setlinecolor(BLACK);

	drawtext(_T("输入参数"), &R1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext(_T("显示结果"), &R2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext(_T("退出"), &R3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rectangle(r1[0], r1[1], r1[2], r1[3]);
	rectangle(r2[0], r2[1], r2[2], r2[3]);
	rectangle(r3[0], r3[1], r3[2], r3[3]);

	MOUSEMSG m;
	
	while (TRUE)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();

			if (m.uMsg == WM_LBUTTONDOWN)
			{
				if (is_button(m.x, m.y) == 1)
				{
					if (InputBox(data, 500, L"输入数据") == IDOK)
					{
						char data_str[500];
						wcstombs(data_str, data, sizeof(data_str));

						int offset = 0;
						sscanf(data_str, "%d", &N);
						offset += strcspn(data_str + offset, "0123456789") + 1;

						for (int i = 0; i < N; i++)
						{
							int n;
							sscanf(data_str + offset, "%d", &n);
							offset += strcspn(data_str + offset, "0123456789") + 1;

							int matrix[max_n][max_n];
							for (int j = 0; j < n; j++)
							{
								for (int k = 0; k < n; k++)
								{
									sscanf(data_str + offset, "%d", &matrix[j][k]);
									offset += strcspn(data_str + offset, "0123456789") + 1;
								}
							}

							offset += strspn(data_str + offset, " \n\r\t"); // 跳过空白字符


							printf("%d\n%d\n", N, n);
							for (int i = 0; i < n; i++)
							{
								for (int j = 0; j < n; j++)
								{
									printf("%d ", matrix[i][j]);
								}
								printf("\n");
							}

							

#if (sel_agorithm == 1)
							possible_winners = duel_force(n, matrix);
#elif (sel_agorithm == 2)
							possible_winners = duel_dynamic(n, matrix);
#endif
							results[num_result++] = possible_winners;
						}
					}
					FlushMouseMsgBuffer();
				}

				else if (is_button(m.x, m.y) == 2)
				{
					setlinestyle(PS_SOLID, 3);
					setfillcolor(WHITE);

					rectangle(10, 100, 470, 350);

					TCHAR results_str[80];
					int i = 0;
					
					while (num_result--)
					{
						_stprintf(results_str, _T("可能胜出的人数：%d"), results[i++]);
						outtextxy(50, 100 + i * 30, results_str);
					}
				}

				else if (is_button(m.x, m.y) == 3)
				{
					Sleep(100);
					closegraph();
					exit(0);
				}

				else
				{
					FlushMouseMsgBuffer();
				}
			}
			

		}
	}
	

	system("pouse");
	closegraph();

	return 0;
}