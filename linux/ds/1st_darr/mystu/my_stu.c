#include <darr.h>
#include <string.h>

struct stu_t {
	char *name;
	int id;
	char sex;
	int age;
	int en;
	int cn;
};

void ls(const void *data, void *arg)
{
	struct stu_t *s = (struct stu_t *)data;

	printf("名字:%s id:%d sex:%c age:%d en:%d cn:%d \n",
			s->name, s->id, s->sex, s->age, s->en, s->cn);
}

void printf_s(void)
{
	printf("-----> 1.name <-----");
	printf("-----> 1.sex  <-----");
	printf("-----> 1.id   <-----");
	printf("-----> 1.age  <-----");
	printf("-----> 1.en   <-----");
	printf("-----> 1.cn   <-----");
	printf("请输入[0 -6]:");
}

int main(void)
{
	DARR *handle = NULL;
	struct stu_t *stu;
	int option, len;
	char buf[128];
	char se[2] = {'F', 'M'};

	handle = darr_create(sizeof(stu), NULL, NULL, NULL);

	while(1)
	{
		printf("==========> 学生成绩管理系统 <==========\n");
		printf("==========> 1. 添加          <==========\n");
		printf("==========> 2. 修改          <==========\n");
		printf("==========> 3. 查询          <==========\n");
		printf("==========> 4. 删除          <==========\n");
		printf("==========> 5. 遍历          <==========\n");
		printf("==========> 6. 存储          <==========\n");
		printf("==========> 7. 加载          <==========\n");
		printf("==========> 8. 退出          <==========\n");
		printf("请输入[1 - 8] :");
		scanf("%d", &option);
		switch(option) 
		{
			case 1:
				printf("请输入名字:");			
				fgets(buf, sizeof(buf), stdin);
				len = strlen(buf);
				if (buf[len - 1] == '\n')
					buf[len - 1] = '\0';
			    else
                    while(getchar() != '\n');
                stu->name = (char *)malloc(len + 1);
				strcpy(stu->name, buf);
				stu->id = rand() % 100;
				stu->age = rand() % 100;
				stu->sex = se[rand() % 2];
				stu->en = rand() % 100;
				stu->cn = rand() % 100;
				
				darr_insert(&stu, APPEND, handle);
				break;
			case 2:
				printf_s();
				scanf("%d",&option);
				

				break;
			case 3:
				printf_s();			
				scanf("%d", &option);

				break;
			case 4:
				printf_s();
				scanf("%d",&option);

				break;
			case 5:
				darr_travel(ls, NULL, handle);
				break;
			case 6:

				break;
			case 7:

				break;
			case 8:
				break;
			default:
				break;
		}
	}

	
	return 0;
}

