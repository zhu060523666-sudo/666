#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <regex.h>
#include <string.h>

int main()
{
    char *zhengze = "<a[^>]+href=\"([^\"]+)\"[^>]*>([^<]+)</a >";
    regex_t biaoda;
    regcomp(&biaoda, zhengze, REG_EXTENDED);

    int wenjian = open("news.html", O_RDONLY);
    struct stat daXiao;
    fstat(wenjian, &daXiao);

    char *neirong = mmap(NULL, daXiao.st_size, PROT_READ, MAP_PRIVATE, wenjian, 0);
    close(wenjian);

    regmatch_t pipei[3];
    char *xianzai = neirong;

    while(regexec(&biaoda, xianzai, 3, pipei, 0) == 0)
    {
        char lianjie[256] = {0};
        char biaoti[256] = {0};

        snprintf(lianjie, sizeof(lianjie), "%.*s",
                 pipei[1].rm_eo - pipei[1].rm_so, xianzai + pipei[1].rm_so);

        snprintf(biaoti, sizeof(biaoti), "%.*s",
                 pipei[2].rm_eo - pipei[2].rm_so, xianzai + pipei[2].rm_so);

        printf("链接：%s  标题：%s\n", lianjie, biaoti);

        xianzai = xianzai + pipei[0].rm_eo;
    }

    regfree(&biaoda);
    munmap(neirong, daXiao.st_size);

    return 0;
}