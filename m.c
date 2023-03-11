#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>//提供对目录读取操作的支持
#include<unistd.h>//提供底层操作支持
int ReadDir(const char *strpathname);//递归打印该目录下的文件或子目录
int MKDIR(const char *pathname);//递归创建目录
int main()
{
    char strcwd[301];
    memset(strcwd,0,sizeof(strcwd));//将无符号字符0复制到strcwd数组的前301个字符，其实就是用0填充整个数组
    getcwd(strcwd,sizeof(strcwd));//获取当前目录的绝对路径，存入strcwd数组
    printf("=%s=\n",strcwd);//打印当前目录的绝对路径
    chdir("/home");//切换目录至home
    memset(strcwd,0,sizeof(strcwd));
    getcwd(strcwd,sizeof(strcwd));
    printf("=%s=\n",strcwd);
    mkdir("/home/hello/c",00755);//创建个具有读/写/执行权限的目录可固定为00755
    rmdir("/home/hello/c");//删除目录c
    ReadDir("/home/hello");//递归获取目录文件
    MKDIR("/home/hello/testc/test2c/test3c");//递归创建目录
    return 0;
}
int MKDIR(const char *pathname)
{
    char strPathName[301];
    int ii;
    for(ii=1;ii<strlen(pathname);ii++)
    {
        if(pathname[ii]!='/')
            continue;
        memset(strPathName,0,sizeof(strPathName));
        strncpy(strPathName,pathname,1);//将pathname里的数据复制到strPathName里,1表示全部复制
        if(access(strPathName,F_OK)==0)//判断该目录是否存在F_OK存在与否R_OK是否可读W_OK是否🉑写X_OK是否🉑执行
            continue;
        if(mkdir(strPathName,00755)!=0)//判断是否🉑创建该目录
            return -1;
    }
    return 0;
}
int ReadDir(const char *strpathname)
{
    DIR *dir;//定义目录指针
    char strchdpath[256];
    if((dir=(opendir(strpathname)))==0)
    {
        printf("打开目录失败\n");
        return -1;
    }
    struct dirent *stdinfo;//定义结构体指针,用于存放该目录下的文件及其子目录信息
    while(1)
    {
        stdinfo=readdir(dir);//遍历dir目录并返回其子目录进入点,失败则返回NULL
        if(stdinfo==0)
            break;
        if(strncpy(stdinfo->d_name,".",1)==0)//将.拷贝进stdinfo->d_name的首位,判断是否属于隐藏文件,从而过滤掉隐藏文件
            continue;
        if(stdinfo->d_type==8)//DT_UNKNAME 0未知类型文件DT_FIFO 1命名管道DT_CHR 2字符设备文件如键盘DT_DIR 4目录文件即文件夹DT_BLK 6块设备文件如硬盘DT_REG 8常规文件DT_LNK 10软连接文件DT_SOCK 12进程通信DT_WHT 14我也不知道啥玩意的东东没查到
        {
            printf("文件名:%s\t文件类型:%d\n",stdinfo->d_name,stdinfo->d_type);
        }
        if(stdinfo->d_type==4)
        {
            sprintf(strchdpath,"%s/%s",strpathname,stdinfo->d_name);//将该目录及其下属目录名输出至strchdpath里
            ReadDir(strchdpath);
        }
    }
    closedir(dir);//关闭该目录
    return 0;
}
