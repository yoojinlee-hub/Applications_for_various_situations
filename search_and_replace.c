#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAXLINE 4096

void search_and_replace(const char *filename, const char *oldstr, const char *newstr)
{
    FILE *fp, *temp;
    char line[MAXLINE];
    int len, n;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Cannot open file: %s\n", filename);
        return;
    }

    temp = tmpfile();
    if (temp == NULL)
    {
        printf("Cannot create temporary file.\n");
        fclose(fp);
        return;
    }

    len = strlen(oldstr);
    while (fgets(line, MAXLINE, fp) != NULL)
    {
        char *p, *q;
        p = line;
        while ((q = strstr(p, oldstr)) != NULL)
        {
            n = q - p;
            if (n > 0 && fwrite(p, n, 1, temp) != 1)
            {
                printf("Error writing to temporary file.\n");
                fclose(fp);
                fclose(temp);
                return;
            }
            if (fwrite(newstr, strlen(newstr), 1, temp) != 1)
            {
                printf("Error writing to temporary file.\n");
                fclose(fp);
                fclose(temp);
                return;
            }
            p = q + len;
        }
        if (*p != '\0' && fwrite(p, strlen(p), 1, temp) != 1)
        {
            printf("Error writing to temporary file.\n");
            fclose(fp);
            fclose(temp);
            return;
        }
    }

    fclose(fp);

    if (fseek(temp, 0, SEEK_SET) != 0)
    {
        printf("Cannot seek to beginning of temporary file.\n");
        fclose(temp);
        return;
    }

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Cannot open file for writing: %s\n", filename);
        fclose(temp);
        return;
    }

    while (fgets(line, MAXLINE, temp) != NULL)
    {
        if (fputs(line, fp) == EOF)
        {
            printf("Error writing to file: %s\n", filename);
            fclose(fp);
            fclose(temp);
            return;
        }
    }

    fclose(fp);
    fclose(temp);
}

void search_dir(const char *dirpath, const char *oldstr, const char *newstr)
{
    DIR *dp;
    struct dirent *dirp;
    struct stat statbuf;
    char path[MAXLINE];

    if ((dp = opendir(dirpath)) == NULL)
    {
        printf("Cannot open directory: %s\n", dirpath);
        return;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        // Ignore current directory and parent directory
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(path, MAXLINE, "%s/%s", dirpath, dirp->d_name);

        // Get file status
        if (lstat(path, &statbuf) != 0)
        {
            printf("Cannot get file status: %s\n", path);
            continue;
        }

        if (// Check if the path is a directory
if (S_ISDIR(statbuf.st_mode)) {
            // Recursively search the directory
            search_dir(path, oldstr, newstr);
}
else if (S_ISREG(statbuf.st_mode)) {
            // Search and replace in the regular file
            search_and_replace(path, oldstr, newstr);
}
else if (S_ISLNK(statbuf.st_mode)) {
            // Get the path of the target file of the symbolic link
            char target[MAXLINE];
            ssize_t len = readlink(path, target, MAXLINE);
            if (len < 0)
            {
                printf("Cannot read target of symbolic link: %s\n", path);
                continue;
            }
            if (len >= MAXLINE)
            {
                printf("Target of symbolic link is too long: %s\n", path);
                continue;
            }
            target[len] = '\0';
            // Get file status of the target file
            if (lstat(target, &statbuf) != 0)
            {
                printf("Cannot get file status of target of symbolic link: %s\n", target);
                continue;
            }

            if (S_ISREG(statbuf.st_mode))
            {
                // Search and replace in the target file of the symbolic link
                search_and_replace(target, oldstr, newstr);
            }
            else
            {
                printf("Target of symbolic link is not a regular file: %s\n", path);
                continue;
            }
    }
    else {
            printf("Unsupported file type: %s\n", path);
            continue;
    }
    }

    closedir(dp);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s directory oldstr newstr\n", argv[0]);
        return 1;
    }
    const char *dirpath = argv[1];
    const char *oldstr = argv[2];
    const char *newstr = argv[3];

    search_dir(dirpath, oldstr, newstr);

    return 0;
}