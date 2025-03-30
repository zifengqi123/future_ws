#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <errno.h>
#include <stddef.h>

struct outbuf
{
    struct outbuf *next;
    char *outptr;
    size_t outbytesleft;
    char buf[256];
};

char *eazyiconv(const char *to, const char *from,
                char *str, size_t str_blen, size_t str_elemsize, size_t out_tailzero_blen, size_t *out_size,
                const char *replchr)
{
    char *retstr = NULL;
    struct outbuf *outhead = NULL;
    struct outbuf *outtail = NULL;
    struct outbuf *outiter = NULL;
    iconv_t cd = NULL;
    char *inptr = str;
    size_t inbytesleft = str_blen;
    int retval = 0;
    int err = 0;
    size_t blocksize = 0;
    size_t totalsize = 0;
    char *retiter = NULL;
    unsigned int chrval = 0;
    iconv_t cdreplchr = NULL;
    char replchrfmtbuf[256] = "";
    char replchrbuf[256] = "";
    char *replchrfmtptr = replchrfmtbuf;
    size_t replchrfmtleft = sizeof replchrfmtbuf;
    char *replchrptr = replchrbuf;
    size_t replchrleft = sizeof replchrbuf;
    int replchr_blen = 0;

    cd = iconv_open(to, from);
    if (cd == (iconv_t)-1)
    {
        goto noclean;
    }

    outhead = outtail = calloc(1, sizeof(struct outbuf));
    if (outtail == NULL)
    {
        goto clean_cd;
    }
    outtail->next = NULL;
    outtail->outptr = outtail->buf;
    outtail->outbytesleft = sizeof outtail->buf;
    memset(outtail->buf, 0, sizeof outtail->buf);

    while (1)
    {
        retval = iconv(cd, &inptr, &inbytesleft, &outtail->outptr, &outtail->outbytesleft);
        if (retval == -1)
            err = errno;
        else
            err = 0;
        switch (err)
        {
            case 0:
                outiter = calloc(1, sizeof(struct outbuf));
                if (outiter == NULL)
                {
                    goto clean_outbufs;
                }
                if (inptr == NULL) // succeeded cleanup iconv
                {
                    goto succeeded;
                }
                else // fully succeeded iconv
                {
                    inptr = NULL; // do cleanup iconv
                    inbytesleft = 0;
                }
                break;
            case EINVAL: // incomplete tail sequence
            case EILSEQ: // invalid sequence
                chrval = 0;
                memcpy(&chrval, inptr, str_elemsize > sizeof chrval ? sizeof chrval : str_elemsize);
                snprintf(replchrfmtbuf, sizeof replchrfmtbuf, replchr, chrval);
                inptr += str_elemsize;
                inbytesleft -= str_elemsize;

                cdreplchr = iconv_open(to, "UTF-8");
                if (cdreplchr == (iconv_t)-1)
                {
                    goto clean_outbufs;
                }
                replchrfmtptr = replchrfmtbuf;
                replchrfmtleft = strlen(replchrfmtbuf);
                replchrptr = replchrbuf;
                replchrleft = sizeof replchrbuf;
                iconv(cdreplchr, &replchrfmtptr, &replchrfmtleft, &replchrptr, &replchrleft);
                iconv(cdreplchr, NULL, NULL, &replchrptr, &replchrleft);
                iconv_close(cdreplchr);
                replchr_blen = replchrptr - replchrbuf;

                if (outtail->outbytesleft < replchr_blen)
                {
                    outiter = calloc(1, sizeof(struct outbuf));
                    if (outiter == NULL)
                    {
                        goto clean_outbufs;
                    }
                    outtail->next = outiter;
                    outtail = outiter;
                    outtail->next = NULL;
                    outtail->outptr = outtail->buf;
                    outtail->outbytesleft = sizeof outtail->buf;
                    memset(outtail->buf, 0, sizeof outtail->buf);
                }
                memcpy(outtail->outptr, replchrbuf, replchr_blen);
                outtail->outptr += replchr_blen;
                outtail->outbytesleft -= replchr_blen;
                break;
            case E2BIG: // no enough space
                outiter = calloc(1, sizeof(struct outbuf));
                if (outiter == NULL)
                {
                    goto clean_outbufs;
                }
                outtail->next = outiter;
                outtail = outiter;
                outtail->next = NULL;
                outtail->outptr = outtail->buf;
                outtail->outbytesleft = sizeof outtail->buf;
                memset(outtail->buf, 0, sizeof outtail->buf);
                break;
            default:
                break;
        }
    }

succeeded:
    totalsize = 0;
    for (outiter = outhead; outiter != NULL; outiter = outiter->next)
    {
        blocksize = outiter->outptr - outiter->buf;
        totalsize += blocksize;
    }
    retstr = calloc(totalsize + out_tailzero_blen, 1);
    if (retstr == NULL)
    {
        goto clean_outbufs;
    }
    retiter = retstr;
    for (outiter = outhead; outiter != NULL; outiter = outiter->next)
    {
        blocksize = outiter->outptr - outiter->buf;
        memcpy(retiter, outiter->buf, blocksize);
        retiter += blocksize;
    }
    memset(retiter, 0, out_tailzero_blen);
    *out_size = totalsize;

clean_outbufs:
    while (outhead != NULL)
    {
        outiter = outhead;
        outhead = outhead->next;
        free(outiter);
    }
    outtail = NULL;
clean_cd:
    iconv_close(cd);
noclean:
    return retstr;
}

int main(int argc, char **argv)
{
    if (argc < 7)
    {
        printf("usage: eiconv_test from_charset from_elemsize to_charset to_elemsize from_file to_file (no utf-16/32)\n");
        return 0;
    }
    FILE *from_file = fopen(argv[5], "rb");
    fseek(from_file, 0, SEEK_END);
    off_t fsize = ftell(from_file);
    fseek(from_file, 0, SEEK_SET);
    char *from_str = malloc(fsize + 1);
    fread(from_str, 1, fsize, from_file);
    fclose(from_file);

    size_t out_size = 0;
    char *to_str = eazyiconv(argv[3], argv[1],
                             from_str, fsize, atoi(argv[2]), atoi(argv[4]), &out_size,
                             "<0x%02X>");

    FILE *to_file = fopen(argv[6], "wb");
    fwrite(to_str, 1, out_size, to_file);
    free(to_str);
    fclose(to_file);
    return 0;
}


