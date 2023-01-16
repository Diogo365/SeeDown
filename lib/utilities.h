#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef char *multi_tok_t;

// Declarations
char *create_string(char *string);
char *lowercase(char *string);
char *uppercase(char *string);
char *trim(char *string);
char *remove_newline(char *string);

char *replace_all(char *string, char find, char replace);
void replace_all_inplace(char *string, char find, char replace);
char *replace(char *string, char *find, char *replace);
void replace_inplace(char *string, char *find, char *replace);

char *concat_strings(int count, ...);
char *string_truncate(char *string, char *start, char *end);

void html_unicode_converter(char *string);
void deci_unicode_converter(char *string);

void destroy_strings(int count, ...);

multi_tok_t init();
char *multi_tok(char *input, char *delimiter);

FILE *open_file(char *filename, char *mode);
void close_file(FILE *file, char *filename);

// Implementations

multi_tok_t s = NULL;

char *create_string(char *string) {
    char *str = (char *) malloc(strlen(string) + 1);
    strcpy(str, string);
    return str;
}

char *lowercase(char *string) {
    char *str = create_string(string);

    for (int i = 0; i < (int) strlen(str); i++) {
        str[i] = tolower(str[i]);
    }

    return str;
}

char *uppercase(char *string) {
    char *str = create_string(string);

    for (int i = 0; i < (int) strlen(str); i++) {
        str[i] = toupper(str[i]);
    }

    return str;
}

char *trim(char *string) {
    int i = 0;
    while (string[i] == ' ') { i++; }

    int j = strlen(string) - 1;
    while (string[j] == ' ') { j--; }

    char *trimmed = (char *) malloc(j - i + 2);
    strncpy(trimmed, string + i, j - i + 1);
    trimmed[j - i + 1] = '\0';

    return trimmed;
}

char *remove_newline(char *string) {
    char *trimmed = trim(string);
    if (trimmed[strlen(trimmed) - 1] == '\n')
        trimmed[strlen(trimmed) - 1] = '\0';
    return trimmed;
}

char *replace_all(char *string, char find, char replace) {
    char *str = create_string(string);

    for (int i = 0; i < (int) strlen(str); i++) {
        if (str[i] == find) {
            str[i] = replace;
        }
    }

    return str;
}

void replace_all_inplace(char *string, char find, char replace) {
    for (int i = 0; i < (int) strlen(string); i++) {
        if (string[i] == find) {
            string[i] = replace;
        }
    }
}

char *replace(char *string, char *find, char *replace) {
    char *str = create_string(string);

    char *start_index = strstr(str, find);
    if (start_index == NULL) {
        free(str);
        return NULL;
    }

    int start_offset = start_index - str;
    int end_offset = start_offset + strlen(find);

    int length = strlen(str) - strlen(find) + strlen(replace);
    char *replaced = (char *) malloc(length + 1);
    strncpy(replaced, str, start_offset);
    replaced[start_offset] = '\0';
    strcat(replaced, replace);
    strcat(replaced, str + end_offset);

    free(str);
    return replaced;
}

void replace_inplace(char *string, char *find, char *replace) {
    char *start_index = strstr(string, find);
    if (start_index == NULL) {
        return;
    }

    int start_offset = start_index - string;
    int end_offset = start_offset + strlen(find);

    int length = strlen(string) - strlen(find) + strlen(replace);
    char *replaced = (char *) malloc(length + 1);
    strncpy(replaced, string, start_offset);
    replaced[start_offset] = '\0';
    strcat(replaced, replace);
    strcat(replaced, string + end_offset);

    strcpy(string, replaced);
    free(replaced);
}

char *concat_strings(int count, ...) {
    va_list args;
    va_start(args, count);

    int length = 0;
    for (int i = 0; i < count; i++) {
        length += strlen(va_arg(args, char *));
    }

    char *str = (char *) malloc(length + 1);
    str[0] = '\0';

    va_start(args, count);
    for (int i = 0; i < count; i++) {
        strcat(str, va_arg(args, char *));
    }

    return str;
}

char *string_truncate(char *string, char *start, char *end) {
    char *start_index = strstr(string, start);
    char *end_index = strstr(start_index + strlen(start), end);

    if (start_index == NULL || end_index == NULL) {
        return NULL;
    }

    int start_offset = strlen(start);
    // int end_offset = strlen(end);

    int length = end_index - start_index - start_offset;
    char *truncated = (char *) malloc(length + 1);
    strncpy(truncated, start_index + start_offset, length);
    truncated[length] = '\0';

    return truncated;
}

void html_unicode_converter(char *string) {
    while (strstr(string, "&quot;") != NULL || strstr(string, "&amp;") != NULL || strstr(string, "&apos;") != NULL || strstr(string, "&lt;") != NULL || strstr(string, "&gt;") != NULL || strstr(string, "&nbsp;") != NULL || strstr(string, "&iexcl;") != NULL || strstr(string, "&cent;") != NULL || strstr(string, "&pound;") != NULL || strstr(string, "&curren;") != NULL || strstr(string, "&yen;") != NULL || strstr(string, "&brvbar;") != NULL || strstr(string, "&sect;") != NULL || strstr(string, "&uml;") != NULL || strstr(string, "&copy;") != NULL || strstr(string, "&ordf;") != NULL || strstr(string, "&laquo;") != NULL || strstr(string, "&not;") != NULL || strstr(string, "&shy;") != NULL || strstr(string, "&reg;") != NULL || strstr(string, "&macr;") != NULL || strstr(string, "&deg;") != NULL || strstr(string, "&plusmn;") != NULL || strstr(string, "&sup2;") != NULL || strstr(string, "&sup3;") != NULL || strstr(string, "&acute;") != NULL || strstr(string, "&micro;") != NULL || strstr(string, "&para;") != NULL || strstr(string, "&middot;") != NULL || strstr(string, "&cedil;") != NULL || strstr(string, "&sup1;") != NULL || strstr(string, "&ordm;") != NULL || strstr(string, "&raquo;") != NULL || strstr(string, "&frac14;") != NULL || strstr(string, "&frac12;") != NULL || strstr(string, "&frac34;") != NULL || strstr(string, "&iquest;") != NULL || strstr(string, "&Agrave;") != NULL || strstr(string, "&Aacute;") != NULL || strstr(string, "&Acirc;") != NULL || strstr(string, "&Atilde;") != NULL || strstr(string, "&Auml;") != NULL || strstr(string, "&Aring;") != NULL || strstr(string, "&AElig;") != NULL || strstr(string, "&Ccedil;") != NULL || strstr(string, "&Egrave;") != NULL || strstr(string, "&Eacute;") != NULL || strstr(string, "&Ecirc;") != NULL || strstr(string, "&Euml;") != NULL || strstr(string, "&Igrave;") != NULL || strstr(string, "&Iacute;") != NULL || strstr(string, "&Icirc;") != NULL || strstr(string, "&Iuml;") != NULL || strstr(string, "&ETH;") != NULL || strstr(string, "&Ntilde;") != NULL || strstr(string, "&Ograve;") != NULL || strstr(string, "&Oacute;") != NULL || strstr(string, "&Ocirc;") != NULL || strstr(string, "&Otilde;") != NULL || strstr(string, "&Ouml;") != NULL || strstr(string, "&times;") != NULL || strstr(string, "&Oslash;") != NULL || strstr(string, "&Ugrave;") != NULL || strstr(string, "&Uacute;") != NULL || strstr(string, "&Ucirc;") != NULL || strstr(string, "&Uuml;") != NULL || strstr(string, "&Yacute;") != NULL || strstr(string, "&THORN;") != NULL || strstr(string, "&szlig;") != NULL || strstr(string, "&agrave;") != NULL || strstr(string, "&aacute;") != NULL || strstr(string, "&acirc;") != NULL || strstr(string, "&atilde;") != NULL || strstr(string, "&auml;") != NULL || strstr(string, "&aring;") != NULL || strstr(string, "&aelig;") != NULL || strstr(string, "&ccedil;") != NULL || strstr(string, "&egrave;") != NULL || strstr(string, "&eacute;") != NULL || strstr(string, "&ecirc;") != NULL || strstr(string, "&euml;") != NULL || strstr(string, "&igrave;") != NULL || strstr(string, "&iacute;") != NULL || strstr(string, "&icirc;") != NULL || strstr(string, "&iuml;") != NULL || strstr(string, "&eth;") != NULL || strstr(string, "&ntilde;") != NULL || strstr(string, "&ograve;") != NULL || strstr(string, "&oacute;") != NULL || strstr(string, "&ocirc;") != NULL || strstr(string, "&otilde;") != NULL || strstr(string, "&ouml;") != NULL || strstr(string, "&divide;") != NULL || strstr(string, "&oslash;") != NULL || strstr(string, "&ugrave;") != NULL || strstr(string, "&uacute;") != NULL || strstr(string, "&ucirc;") != NULL || strstr(string, "&uuml;") != NULL || strstr(string, "&yacute;") != NULL || strstr(string, "&thorn;") != NULL || strstr(string, "&yuml;") != NULL || strstr(string, "&OElig;") != NULL || strstr(string, "&oelig;") != NULL || strstr(string, "&Scaron;") != NULL || strstr(string, "&scaron;") != NULL || strstr(string, "&Yuml;") != NULL || strstr(string, "&fnof;") != NULL || strstr(string, "&circ;") != NULL || strstr(string, "&tilde;") != NULL || strstr(string, "&Alpha;") != NULL || strstr(string, "&Beta;") != NULL || strstr(string, "&Gamma;") != NULL || strstr(string, "&Delta;") != NULL || strstr(string, "&Epsilon;") != NULL || strstr(string, "&Zeta;") != NULL || strstr(string, "&Eta;") != NULL || strstr(string, "&Theta;") != NULL || strstr(string, "&Iota;") != NULL || strstr(string, "&Kappa;") != NULL || strstr(string, "&Lambda;") != NULL || strstr(string, "&Mu;") != NULL || strstr(string, "&Nu;") != NULL || strstr(string, "&Xi;") != NULL || strstr(string, "&Omicron;") != NULL || strstr(string, "&Pi;") != NULL || strstr(string, "&Rho;") != NULL || strstr(string, "&Sigma;") != NULL || strstr(string, "&Tau;") != NULL || strstr(string, "&Upsilon;") != NULL || strstr(string, "&Phi;") != NULL || strstr(string, "&Chi;") != NULL || strstr(string, "&Psi;") != NULL || strstr(string, "&Omega;") != NULL || strstr(string, "&alpha;") != NULL || strstr(string, "&beta;") != NULL || strstr(string, "&gamma;") != NULL || strstr(string, "&delta;") != NULL || strstr(string, "&epsilon;") != NULL || strstr(string, "&zeta;") != NULL || strstr(string, "&eta;") != NULL || strstr(string, "&theta;") != NULL || strstr(string, "&iota;") != NULL || strstr(string, "&kappa;") != NULL || strstr(string, "&lambda;") != NULL || strstr(string, "&mu;") != NULL || strstr(string, "&nu;") != NULL || strstr(string, "&xi;") != NULL || strstr(string, "&omicron;") != NULL || strstr(string, "&pi;") != NULL || strstr(string, "&rho;") != NULL || strstr(string, "&sigmaf;") != NULL || strstr(string, "&sigma;") != NULL || strstr(string, "&tau;") != NULL || strstr(string, "&upsilon;") != NULL || strstr(string, "&phi;") != NULL || strstr(string, "&chi;") != NULL || strstr(string, "&psi;") != NULL || strstr(string, "&omega;") != NULL || strstr(string, "&thetasym;") != NULL || strstr(string, "&upsih;") != NULL || strstr(string, "&piv;") != NULL || strstr(string, "&ensp;") != NULL || strstr(string, "&emsp;") != NULL || strstr(string, "&thinsp;") != NULL || strstr(string, "&zwnj;") != NULL || strstr(string, "&zwj;") != NULL || strstr(string, "&lrm;") != NULL || strstr(string, "&rlm;") != NULL || strstr(string, "&ndash;") != NULL || strstr(string, "&mdash;") != NULL || strstr(string, "&lsquo;") != NULL || strstr(string, "&rsquo;") != NULL || strstr(string, "&sbquo;") != NULL || strstr(string, "&ldquo;") != NULL || strstr(string, "&rdquo;") != NULL || strstr(string, "&bdquo;") != NULL || strstr(string, "&dagger;") != NULL || strstr(string, "&Dagger;") != NULL || strstr(string, "&bull;") != NULL || strstr(string, "&hellip;") != NULL || strstr(string, "&permil;") != NULL || strstr(string, "&prime;") != NULL || strstr(string, "&Prime;") != NULL || strstr(string, "&lsaquo;") != NULL || strstr(string, "&rsaquo;") != NULL || strstr(string, "&oline;") != NULL || strstr(string, "&frasl;") != NULL || strstr(string, "&euro;") != NULL || strstr(string, "&image;") != NULL || strstr(string, "&weierp;") != NULL || strstr(string, "&real;") != NULL || strstr(string, "&trade;") != NULL || strstr(string, "&alefsym;") != NULL || strstr(string, "&larr;") != NULL || strstr(string, "&uarr;") != NULL || strstr(string, "&rarr;") != NULL || strstr(string, "&darr;") != NULL || strstr(string, "&harr;") != NULL || strstr(string, "&crarr;") != NULL || strstr(string, "&lArr;") != NULL || strstr(string, "&uArr;") != NULL || strstr(string, "&rArr;") != NULL || strstr(string, "&dArr;") != NULL || strstr(string, "&hArr;") != NULL || strstr(string, "&forall;") != NULL || strstr(string, "&part;") != NULL || strstr(string, "&exist;") != NULL || strstr(string, "&empty;") != NULL || strstr(string, "&nabla;") != NULL || strstr(string, "&isin;") != NULL || strstr(string, "&notin;") != NULL || strstr(string, "&ni;") != NULL || strstr(string, "&prod;") != NULL || strstr(string, "&sum;") != NULL || strstr(string, "&minus;") != NULL || strstr(string, "&lowast;") != NULL || strstr(string, "&radic;") != NULL || strstr(string, "&prop;") != NULL || strstr(string, "&infin;") != NULL || strstr(string, "&ang;") != NULL || strstr(string, "&and;") != NULL || strstr(string, "&or;") != NULL || strstr(string, "&cap;") != NULL || strstr(string, "&cup;") != NULL || strstr(string, "&int;") != NULL || strstr(string, "&there4;") != NULL || strstr(string, "&sim;") != NULL || strstr(string, "&cong;") != NULL || strstr(string, "&asymp;") != NULL || strstr(string, "&ne;") != NULL || strstr(string, "&equiv;") != NULL || strstr(string, "&le;") != NULL || strstr(string, "&ge;") != NULL || strstr(string, "&sub;") != NULL || strstr(string, "&sup;") != NULL || strstr(string, "&nsub;") != NULL || strstr(string, "&sube;") != NULL || strstr(string, "&supe;") != NULL || strstr(string, "&oplus;") != NULL || strstr(string, "&otimes;") != NULL || strstr(string, "&perp;") != NULL || strstr(string, "&sdot;") != NULL || strstr(string, "&lceil;") != NULL || strstr(string, "&rceil;") != NULL || strstr(string, "&lfloor;") != NULL || strstr(string, "&rfloor;") != NULL || strstr(string, "&loz;") != NULL || strstr(string, "&spades;") != NULL || strstr(string, "&clubs;") != NULL || strstr(string, "&hearts;") != NULL || strstr(string, "&diams;") != NULL || strstr(string, "&lang;") != NULL || strstr(string, "&rang;") != NULL) {
        replace_inplace(string, "&quot;", "\"");
        replace_inplace(string, "&amp;", "&");
        replace_inplace(string, "&apos;", "'");
        replace_inplace(string, "&lt;", "<");
        replace_inplace(string, "&gt;", ">");
        replace_inplace(string, "&nbsp;", " ");
        replace_inplace(string, "&iexcl;", "¡");
        replace_inplace(string, "&cent;", "¢");
        replace_inplace(string, "&pound;", "£");
        replace_inplace(string, "&curren;", "¤");
        replace_inplace(string, "&yen;", "¥");
        replace_inplace(string, "&brvbar;", "¦");
        replace_inplace(string, "&sect;", "§");
        replace_inplace(string, "&uml;", "¨");
        replace_inplace(string, "&copy;", "©");
        replace_inplace(string, "&ordf;", "ª");
        replace_inplace(string, "&laquo;", "«");
        replace_inplace(string, "&not;", "¬");
        replace_inplace(string, "&shy;", "­");
        replace_inplace(string, "&reg;", "®");
        replace_inplace(string, "&macr;", "¯");
        replace_inplace(string, "&deg;", "°");
        replace_inplace(string, "&plusmn;", "±");
        replace_inplace(string, "&sup2;", "²");
        replace_inplace(string, "&sup3;", "³");
        replace_inplace(string, "&acute;", "´");
        replace_inplace(string, "&micro;", "µ");
        replace_inplace(string, "&para;", "¶");
        replace_inplace(string, "&middot;", "·");
        replace_inplace(string, "&cedil;", "¸");
        replace_inplace(string, "&sup1;", "¹");
        replace_inplace(string, "&ordm;", "º");
        replace_inplace(string, "&raquo;", "»");
        replace_inplace(string, "&frac14;", "¼");
        replace_inplace(string, "&frac12;", "½");
        replace_inplace(string, "&frac34;", "¾");
        replace_inplace(string, "&iquest;", "¿");
        replace_inplace(string, "&Agrave;", "À");
        replace_inplace(string, "&Aacute;", "Á");
        replace_inplace(string, "&Acirc;", "Â");
        replace_inplace(string, "&Atilde;", "Ã");
        replace_inplace(string, "&Auml;", "Ä");
        replace_inplace(string, "&Aring;", "Å");
        replace_inplace(string, "&AElig;", "Æ");
        replace_inplace(string, "&Ccedil;", "Ç");
        replace_inplace(string, "&Egrave;", "È");
        replace_inplace(string, "&Eacute;", "É");
        replace_inplace(string, "&Ecirc;", "Ê");
        replace_inplace(string, "&Euml;", "Ë");
        replace_inplace(string, "&Igrave;", "Ì");
        replace_inplace(string, "&Iacute;", "Í");
        replace_inplace(string, "&Icirc;", "Î");
        replace_inplace(string, "&Iuml;", "Ï");
        replace_inplace(string, "&ETH;", "Ð");
        replace_inplace(string, "&Ntilde;", "Ñ");
        replace_inplace(string, "&Ograve;", "Ò");
        replace_inplace(string, "&Oacute;", "Ó");
        replace_inplace(string, "&Ocirc;", "Ô");
        replace_inplace(string, "&Otilde;", "Õ");
        replace_inplace(string, "&Ouml;", "Ö");
        replace_inplace(string, "&times;", "×");
        replace_inplace(string, "&Oslash;", "Ø");
        replace_inplace(string, "&Ugrave;", "Ù");
        replace_inplace(string, "&Uacute;", "Ú");
        replace_inplace(string, "&Ucirc;", "Û");
        replace_inplace(string, "&Uuml;", "Ü");
        replace_inplace(string, "&Yacute;", "Ý");
        replace_inplace(string, "&THORN;", "Þ");
        replace_inplace(string, "&szlig;", "ß");
        replace_inplace(string, "&agrave;", "à");
        replace_inplace(string, "&aacute;", "á");
        replace_inplace(string, "&acirc;", "â");
        replace_inplace(string, "&atilde;", "ã");
        replace_inplace(string, "&auml;", "ä");
        replace_inplace(string, "&aring;", "å");
        replace_inplace(string, "&aelig;", "æ");
        replace_inplace(string, "&ccedil;", "ç");
        replace_inplace(string, "&egrave;", "è");
        replace_inplace(string, "&eacute;", "é");
        replace_inplace(string, "&ecirc;", "ê");
        replace_inplace(string, "&euml;", "ë");
        replace_inplace(string, "&igrave;", "ì");
        replace_inplace(string, "&iacute;", "í");
        replace_inplace(string, "&icirc;", "î");
        replace_inplace(string, "&iuml;", "ï");
        replace_inplace(string, "&eth;", "ð");
        replace_inplace(string, "&ntilde;", "ñ");
        replace_inplace(string, "&ograve;", "ò");
        replace_inplace(string, "&oacute;", "ó");
        replace_inplace(string, "&ocirc;", "ô");
        replace_inplace(string, "&otilde;", "õ");
        replace_inplace(string, "&ouml;", "ö");
        replace_inplace(string, "&divide;", "÷");
        replace_inplace(string, "&oslash;", "ø");
        replace_inplace(string, "&ugrave;", "ù");
        replace_inplace(string, "&uacute;", "ú");
        replace_inplace(string, "&ucirc;", "û");
        replace_inplace(string, "&uuml;", "ü");
        replace_inplace(string, "&yacute;", "ý");
        replace_inplace(string, "&thorn;", "þ");
        replace_inplace(string, "&yuml;", "ÿ");
        replace_inplace(string, "&OElig;", "Œ");
        replace_inplace(string, "&oelig;", "œ");
        replace_inplace(string, "&Scaron;", "Š");
        replace_inplace(string, "&scaron;", "š");
        replace_inplace(string, "&Yuml;", "Ÿ");
        replace_inplace(string, "&fnof;", "ƒ");
        replace_inplace(string, "&circ;", "ˆ");
        replace_inplace(string, "&tilde;", "˜");
        replace_inplace(string, "&Alpha;", "Α");
        replace_inplace(string, "&Beta;", "Β");
        replace_inplace(string, "&Gamma;", "Γ");
        replace_inplace(string, "&Delta;", "Δ");
        replace_inplace(string, "&Epsilon;", "Ε");
        replace_inplace(string, "&Zeta;", "Ζ");
        replace_inplace(string, "&Eta;", "Η");
        replace_inplace(string, "&Theta;", "Θ");
        replace_inplace(string, "&Iota;", "Ι");
        replace_inplace(string, "&Kappa;", "Κ");
        replace_inplace(string, "&Lambda;", "Λ");
        replace_inplace(string, "&Mu;", "Μ");
        replace_inplace(string, "&Nu;", "Ν");
        replace_inplace(string, "&Xi;", "Ξ");
        replace_inplace(string, "&Omicron;", "Ο");
        replace_inplace(string, "&Pi;", "Π");
        replace_inplace(string, "&Rho;", "Ρ");
        replace_inplace(string, "&Sigma;", "Σ");
        replace_inplace(string, "&Tau;", "Τ");
        replace_inplace(string, "&Upsilon;", "Υ");
        replace_inplace(string, "&Phi;", "Φ");
        replace_inplace(string, "&Chi;", "Χ");
        replace_inplace(string, "&Psi;", "Ψ");
        replace_inplace(string, "&Omega;", "Ω");
        replace_inplace(string, "&alpha;", "α");
        replace_inplace(string, "&beta;", "β");
        replace_inplace(string, "&gamma;", "γ");
        replace_inplace(string, "&delta;", "δ");
        replace_inplace(string, "&epsilon;", "ε");
        replace_inplace(string, "&zeta;", "ζ");
        replace_inplace(string, "&eta;", "η");
        replace_inplace(string, "&theta;", "θ");
        replace_inplace(string, "&iota;", "ι");
        replace_inplace(string, "&kappa;", "κ");
        replace_inplace(string, "&lambda;", "λ");
        replace_inplace(string, "&mu;", "μ");
        replace_inplace(string, "&nu;", "ν");
        replace_inplace(string, "&xi;", "ξ");
        replace_inplace(string, "&omicron;", "ο");
        replace_inplace(string, "&pi;", "π");
        replace_inplace(string, "&rho;", "ρ");
        replace_inplace(string, "&sigmaf;", "ς");
        replace_inplace(string, "&sigma;", "σ");
        replace_inplace(string, "&tau;", "τ");
        replace_inplace(string, "&upsilon;", "υ");
        replace_inplace(string, "&phi;", "φ");
        replace_inplace(string, "&chi;", "χ");
        replace_inplace(string, "&psi;", "ψ");
        replace_inplace(string, "&omega;", "ω");
        replace_inplace(string, "&thetasym;", "ϑ");
        replace_inplace(string, "&upsih;", "ϒ");
        replace_inplace(string, "&piv;", "ϖ");
        replace_inplace(string, "&ensp;", " ");
        replace_inplace(string, "&emsp;", " ");
        replace_inplace(string, "&thinsp;", " ");
        replace_inplace(string, "&zwnj;", "‌");
        replace_inplace(string, "&zwj;", "‍");
        replace_inplace(string, "&lrm;", "‎");
        replace_inplace(string, "&rlm;", "‏");
        replace_inplace(string, "&ndash;", "–");
        replace_inplace(string, "&mdash;", "—");
        replace_inplace(string, "&lsquo;", "‘");
        replace_inplace(string, "&rsquo;", "’");
        replace_inplace(string, "&sbquo;", "‚");
        replace_inplace(string, "&ldquo;", "“");
        replace_inplace(string, "&rdquo;", "”");
        replace_inplace(string, "&bdquo;", "„");
        replace_inplace(string, "&dagger;", "†");
        replace_inplace(string, "&Dagger;", "‡");
        replace_inplace(string, "&bull;", "•");
        replace_inplace(string, "&hellip;", "…");
        replace_inplace(string, "&permil;", "‰");
        replace_inplace(string, "&prime;", "′");
        replace_inplace(string, "&Prime;", "″");
        replace_inplace(string, "&lsaquo;", "‹");
        replace_inplace(string, "&rsaquo;", "›");
        replace_inplace(string, "&oline;", "‾");
        replace_inplace(string, "&frasl;", "⁄");
        replace_inplace(string, "&euro;", "€");
        replace_inplace(string, "&image;", "ℑ");
        replace_inplace(string, "&weierp;", "℘");
        replace_inplace(string, "&real;", "ℜ");
        replace_inplace(string, "&trade;", "™");
        replace_inplace(string, "&alefsym;", "ℵ");
        replace_inplace(string, "&larr;", "←");
        replace_inplace(string, "&uarr;", "↑");
        replace_inplace(string, "&rarr;", "→");
        replace_inplace(string, "&darr;", "↓");
        replace_inplace(string, "&harr;", "↔");
        replace_inplace(string, "&crarr;", "↵");
        replace_inplace(string, "&lArr;", "⇐");
        replace_inplace(string, "&uArr;", "⇑");
        replace_inplace(string, "&rArr;", "⇒");
        replace_inplace(string, "&dArr;", "⇓");
        replace_inplace(string, "&hArr;", "⇔");
        replace_inplace(string, "&forall;", "∀");
        replace_inplace(string, "&part;", "∂");
        replace_inplace(string, "&exist;", "∃");
        replace_inplace(string, "&empty;", "∅");
        replace_inplace(string, "&nabla;", "∇");
        replace_inplace(string, "&isin;", "∈");
        replace_inplace(string, "&notin;", "∉");
        replace_inplace(string, "&ni;", "∋");
        replace_inplace(string, "&prod;", "∏");
        replace_inplace(string, "&sum;", "∑");
        replace_inplace(string, "&minus;", "−");
        replace_inplace(string, "&lowast;", "∗");
        replace_inplace(string, "&radic;", "√");
        replace_inplace(string, "&prop;", "∝");
        replace_inplace(string, "&infin;", "∞");
        replace_inplace(string, "&ang;", "∠");
        replace_inplace(string, "&and;", "∧");
        replace_inplace(string, "&or;", "∨");
        replace_inplace(string, "&cap;", "∩");
        replace_inplace(string, "&cup;", "∪");
        replace_inplace(string, "&int;", "∫");
        replace_inplace(string, "&there4;", "∴");
        replace_inplace(string, "&sim;", "∼");
        replace_inplace(string, "&cong;", "≅");
        replace_inplace(string, "&asymp;", "≈");
        replace_inplace(string, "&ne;", "≠");
        replace_inplace(string, "&equiv;", "≡");
        replace_inplace(string, "&le;", "≤");
        replace_inplace(string, "&ge;", "≥");
        replace_inplace(string, "&sub;", "⊂");
        replace_inplace(string, "&sup;", "⊃");
        replace_inplace(string, "&nsub;", "⊄");
        replace_inplace(string, "&sube;", "⊆");
        replace_inplace(string, "&supe;", "⊇");
        replace_inplace(string, "&oplus;", "⊕");
        replace_inplace(string, "&otimes;", "⊗");
        replace_inplace(string, "&perp;", "⊥");
        replace_inplace(string, "&sdot;", "⋅");
        replace_inplace(string, "&lceil;", "⌈");
        replace_inplace(string, "&rceil;", "⌉");
        replace_inplace(string, "&lfloor;", "⌊");
        replace_inplace(string, "&rfloor;", "⌋");
        replace_inplace(string, "&loz;", "◊");
        replace_inplace(string, "&spades;", "♠");
        replace_inplace(string, "&clubs;", "♣");
        replace_inplace(string, "&hearts;", "♥");
        replace_inplace(string, "&diams;", "♦");
        replace_inplace(string, "&lang;", "⟨");
        replace_inplace(string, "&rang;", "⟩");
    }
}

void deci_unicode_converter(char *string) {
    while (strstr(string, "&#34") != NULL || strstr(string, "&#38") != NULL || strstr(string, "&#39") != NULL || strstr(string, "&#60") != NULL || strstr(string, "&#62") != NULL || strstr(string, "&#160") != NULL || strstr(string, "&#161") != NULL || strstr(string, "&#162") != NULL || strstr(string, "&#163") != NULL || strstr(string, "&#164") != NULL || strstr(string, "&#165") != NULL || strstr(string, "&#166") != NULL || strstr(string, "&#167") != NULL || strstr(string, "&#168") != NULL || strstr(string, "&#169") != NULL || strstr(string, "&#170") != NULL || strstr(string, "&#171") != NULL || strstr(string, "&#172") != NULL || strstr(string, "&#173") != NULL || strstr(string, "&#174") != NULL || strstr(string, "&#175") != NULL || strstr(string, "&#176") != NULL || strstr(string, "&#177") != NULL || strstr(string, "&#178") != NULL || strstr(string, "&#179") != NULL || strstr(string, "&#180") != NULL || strstr(string, "&#181") != NULL || strstr(string, "&#182") != NULL || strstr(string, "&#183") != NULL || strstr(string, "&#184") != NULL || strstr(string, "&#185") != NULL || strstr(string, "&#186") != NULL || strstr(string, "&#187") != NULL || strstr(string, "&#188") != NULL || strstr(string, "&#189") != NULL || strstr(string, "&#190") != NULL || strstr(string, "&#191") != NULL || strstr(string, "&#192") != NULL || strstr(string, "&#193") != NULL || strstr(string, "&#194") != NULL || strstr(string, "&#195") != NULL || strstr(string, "&#196") != NULL || strstr(string, "&#197") != NULL || strstr(string, "&#198") != NULL || strstr(string, "&#199") != NULL || strstr(string, "&#200") != NULL || strstr(string, "&#201") != NULL || strstr(string, "&#202") != NULL || strstr(string, "&#203") != NULL || strstr(string, "&#204") != NULL || strstr(string, "&#205") != NULL || strstr(string, "&#206") != NULL || strstr(string, "&#207") != NULL || strstr(string, "&#208") != NULL || strstr(string, "&#209") != NULL || strstr(string, "&#210") != NULL || strstr(string, "&#211") != NULL || strstr(string, "&#212") != NULL || strstr(string, "&#213") != NULL || strstr(string, "&#214") != NULL || strstr(string, "&#215") != NULL || strstr(string, "&#216") != NULL || strstr(string, "&#217") != NULL || strstr(string, "&#218") != NULL || strstr(string, "&#219") != NULL || strstr(string, "&#220") != NULL || strstr(string, "&#221") != NULL || strstr(string, "&#222") != NULL || strstr(string, "&#223") != NULL || strstr(string, "&#224") != NULL || strstr(string, "&#225") != NULL || strstr(string, "&#226") != NULL || strstr(string, "&#227") != NULL || strstr(string, "&#228") != NULL || strstr(string, "&#229") != NULL || strstr(string, "&#230") != NULL || strstr(string, "&#231") != NULL || strstr(string, "&#232") != NULL || strstr(string, "&#233") != NULL || strstr(string, "&#234") != NULL || strstr(string, "&#235") != NULL || strstr(string, "&#236") != NULL || strstr(string, "&#237") != NULL || strstr(string, "&#238") != NULL || strstr(string, "&#239") != NULL || strstr(string, "&#240") != NULL || strstr(string, "&#241") != NULL || strstr(string, "&#242") != NULL || strstr(string, "&#243") != NULL || strstr(string, "&#244") != NULL || strstr(string, "&#245") != NULL || strstr(string, "&#246") != NULL || strstr(string, "&#247") != NULL || strstr(string, "&#248") != NULL || strstr(string, "&#249") != NULL || strstr(string, "&#250") != NULL || strstr(string, "&#251") != NULL || strstr(string, "&#252") != NULL || strstr(string, "&#253") != NULL || strstr(string, "&#254") != NULL || strstr(string, "&#255") != NULL || strstr(string, "&#338") != NULL || strstr(string, "&#339") != NULL || strstr(string, "&#352") != NULL || strstr(string, "&#353") != NULL || strstr(string, "&#376") != NULL || strstr(string, "&#402") != NULL || strstr(string, "&#710") != NULL || strstr(string, "&#732") != NULL || strstr(string, "&#913") != NULL || strstr(string, "&#914") != NULL || strstr(string, "&#915") != NULL || strstr(string, "&#916") != NULL || strstr(string, "&#917") != NULL || strstr(string, "&#918") != NULL || strstr(string, "&#919") != NULL || strstr(string, "&#920") != NULL || strstr(string, "&#921") != NULL || strstr(string, "&#922") != NULL || strstr(string, "&#923") != NULL || strstr(string, "&#924") != NULL || strstr(string, "&#925") != NULL || strstr(string, "&#926") != NULL || strstr(string, "&#927") != NULL || strstr(string, "&#928") != NULL || strstr(string, "&#929") != NULL || strstr(string, "&#931") != NULL || strstr(string, "&#932") != NULL || strstr(string, "&#933") != NULL || strstr(string, "&#934") != NULL || strstr(string, "&#935") != NULL || strstr(string, "&#936") != NULL || strstr(string, "&#937") != NULL || strstr(string, "&#945") != NULL || strstr(string, "&#946") != NULL || strstr(string, "&#947") != NULL || strstr(string, "&#948") != NULL || strstr(string, "&#949") != NULL || strstr(string, "&#950") != NULL || strstr(string, "&#951") != NULL || strstr(string, "&#952") != NULL || strstr(string, "&#953") != NULL || strstr(string, "&#954") != NULL || strstr(string, "&#955") != NULL || strstr(string, "&#956") != NULL || strstr(string, "&#957") != NULL || strstr(string, "&#958") != NULL || strstr(string, "&#959") != NULL || strstr(string, "&#960") != NULL || strstr(string, "&#961") != NULL || strstr(string, "&#962") != NULL || strstr(string, "&#963") != NULL || strstr(string, "&#964") != NULL || strstr(string, "&#965") != NULL || strstr(string, "&#966") != NULL || strstr(string, "&#967") != NULL || strstr(string, "&#968") != NULL || strstr(string, "&#969") != NULL || strstr(string, "&#977") != NULL || strstr(string, "&#978") != NULL || strstr(string, "&#982") != NULL || strstr(string, "&#8194") != NULL || strstr(string, "&#8195") != NULL || strstr(string, "&#8201") != NULL || strstr(string, "&#8204") != NULL || strstr(string, "&#8205") != NULL || strstr(string, "&#8206") != NULL || strstr(string, "&#8207") != NULL || strstr(string, "&#8211") != NULL || strstr(string, "&#8212") != NULL || strstr(string, "&#8216") != NULL || strstr(string, "&#8217") != NULL || strstr(string, "&#8218") != NULL || strstr(string, "&#8220") != NULL || strstr(string, "&#8221") != NULL || strstr(string, "&#8222") != NULL || strstr(string, "&#8224") != NULL || strstr(string, "&#8225") != NULL || strstr(string, "&#8226") != NULL || strstr(string, "&#8230") != NULL || strstr(string, "&#8240") != NULL || strstr(string, "&#8242") != NULL || strstr(string, "&#8243") != NULL || strstr(string, "&#8249") != NULL || strstr(string, "&#8250") != NULL || strstr(string, "&#8254") != NULL || strstr(string, "&#8260") != NULL || strstr(string, "&#8364") != NULL || strstr(string, "&#8465") != NULL || strstr(string, "&#8472") != NULL || strstr(string, "&#8476") != NULL || strstr(string, "&#8482") != NULL || strstr(string, "&#8501") != NULL || strstr(string, "&#8592") != NULL || strstr(string, "&#8593") != NULL || strstr(string, "&#8594") != NULL || strstr(string, "&#8595") != NULL || strstr(string, "&#8596") != NULL || strstr(string, "&#8629") != NULL || strstr(string, "&#8656") != NULL || strstr(string, "&#8657") != NULL || strstr(string, "&#8658") != NULL || strstr(string, "&#8659") != NULL || strstr(string, "&#8660") != NULL || strstr(string, "&#8704") != NULL || strstr(string, "&#8706") != NULL || strstr(string, "&#8707") != NULL || strstr(string, "&#8709") != NULL || strstr(string, "&#8711") != NULL || strstr(string, "&#8712") != NULL || strstr(string, "&#8713") != NULL || strstr(string, "&#8715") != NULL || strstr(string, "&#8719") != NULL || strstr(string, "&#8721") != NULL || strstr(string, "&#8722") != NULL || strstr(string, "&#8727") != NULL || strstr(string, "&#8730") != NULL || strstr(string, "&#8733") != NULL || strstr(string, "&#8734") != NULL || strstr(string, "&#8736") != NULL || strstr(string, "&#8743") != NULL || strstr(string, "&#8744") != NULL || strstr(string, "&#8745") != NULL || strstr(string, "&#8746") != NULL || strstr(string, "&#8747") != NULL || strstr(string, "&#8756") != NULL || strstr(string, "&#8764") != NULL || strstr(string, "&#8773") != NULL || strstr(string, "&#8776") != NULL || strstr(string, "&#8800") != NULL || strstr(string, "&#8801") != NULL || strstr(string, "&#8804") != NULL || strstr(string, "&#8805") != NULL || strstr(string, "&#8834") != NULL || strstr(string, "&#8835") != NULL || strstr(string, "&#8836") != NULL || strstr(string, "&#8838") != NULL || strstr(string, "&#8839") != NULL || strstr(string, "&#8853") != NULL || strstr(string, "&#8855") != NULL || strstr(string, "&#8869") != NULL || strstr(string, "&#8901") != NULL || strstr(string, "&#8968") != NULL || strstr(string, "&#8969") != NULL || strstr(string, "&#8970") != NULL || strstr(string, "&#8971") != NULL || strstr(string, "&#9674") != NULL || strstr(string, "&#9824") != NULL || strstr(string, "&#9827") != NULL || strstr(string, "&#9829") != NULL || strstr(string, "&#9830") != NULL || strstr(string, "&#10216") != NULL || strstr(string, "&#10217") != NULL) {
        replace_inplace(string, "&#34", "\"");
        replace_inplace(string, "&#38", "&");
        replace_inplace(string, "&#39", "'");
        replace_inplace(string, "&#60", "<");
        replace_inplace(string, "&#62", ">");
        replace_inplace(string, "&#160", " ");
        replace_inplace(string, "&#161", "¡");
        replace_inplace(string, "&#162", "¢");
        replace_inplace(string, "&#163", "£");
        replace_inplace(string, "&#164", "¤");
        replace_inplace(string, "&#165", "¥");
        replace_inplace(string, "&#166", "¦");
        replace_inplace(string, "&#167", "§");
        replace_inplace(string, "&#168", "¨");
        replace_inplace(string, "&#169", "©");
        replace_inplace(string, "&#170", "ª");
        replace_inplace(string, "&#171", "«");
        replace_inplace(string, "&#172", "¬");
        replace_inplace(string, "&#173", "­");
        replace_inplace(string, "&#174", "®");
        replace_inplace(string, "&#175", "¯");
        replace_inplace(string, "&#176", "°");
        replace_inplace(string, "&#177", "±");
        replace_inplace(string, "&#178", "²");
        replace_inplace(string, "&#179", "³");
        replace_inplace(string, "&#180", "´");
        replace_inplace(string, "&#181", "µ");
        replace_inplace(string, "&#182", "¶");
        replace_inplace(string, "&#183", "·");
        replace_inplace(string, "&#184", "¸");
        replace_inplace(string, "&#185", "¹");
        replace_inplace(string, "&#186", "º");
        replace_inplace(string, "&#187", "»");
        replace_inplace(string, "&#188", "¼");
        replace_inplace(string, "&#189", "½");
        replace_inplace(string, "&#190", "¾");
        replace_inplace(string, "&#191", "¿");
        replace_inplace(string, "&#192", "À");
        replace_inplace(string, "&#193", "Á");
        replace_inplace(string, "&#194", "Â");
        replace_inplace(string, "&#195", "Ã");
        replace_inplace(string, "&#196", "Ä");
        replace_inplace(string, "&#197", "Å");
        replace_inplace(string, "&#198", "Æ");
        replace_inplace(string, "&#199", "Ç");
        replace_inplace(string, "&#200", "È");
        replace_inplace(string, "&#201", "É");
        replace_inplace(string, "&#202", "Ê");
        replace_inplace(string, "&#203", "Ë");
        replace_inplace(string, "&#204", "Ì");
        replace_inplace(string, "&#205", "Í");
        replace_inplace(string, "&#206", "Î");
        replace_inplace(string, "&#207", "Ï");
        replace_inplace(string, "&#208", "Ð");
        replace_inplace(string, "&#209", "Ñ");
        replace_inplace(string, "&#210", "Ò");
        replace_inplace(string, "&#211", "Ó");
        replace_inplace(string, "&#212", "Ô");
        replace_inplace(string, "&#213", "Õ");
        replace_inplace(string, "&#214", "Ö");
        replace_inplace(string, "&#215", "×");
        replace_inplace(string, "&#216", "Ø");
        replace_inplace(string, "&#217", "Ù");
        replace_inplace(string, "&#218", "Ú");
        replace_inplace(string, "&#219", "Û");
        replace_inplace(string, "&#220", "Ü");
        replace_inplace(string, "&#221", "Ý");
        replace_inplace(string, "&#222", "Þ");
        replace_inplace(string, "&#223", "ß");
        replace_inplace(string, "&#224", "à");
        replace_inplace(string, "&#225", "á");
        replace_inplace(string, "&#226", "â");
        replace_inplace(string, "&#227", "ã");
        replace_inplace(string, "&#228", "ä");
        replace_inplace(string, "&#229", "å");
        replace_inplace(string, "&#230", "æ");
        replace_inplace(string, "&#231", "ç");
        replace_inplace(string, "&#232", "è");
        replace_inplace(string, "&#233", "é");
        replace_inplace(string, "&#234", "ê");
        replace_inplace(string, "&#235", "ë");
        replace_inplace(string, "&#236", "ì");
        replace_inplace(string, "&#237", "í");
        replace_inplace(string, "&#238", "î");
        replace_inplace(string, "&#239", "ï");
        replace_inplace(string, "&#240", "ð");
        replace_inplace(string, "&#241", "ñ");
        replace_inplace(string, "&#242", "ò");
        replace_inplace(string, "&#243", "ó");
        replace_inplace(string, "&#244", "ô");
        replace_inplace(string, "&#245", "õ");
        replace_inplace(string, "&#246", "ö");
        replace_inplace(string, "&#247", "÷");
        replace_inplace(string, "&#248", "ø");
        replace_inplace(string, "&#249", "ù");
        replace_inplace(string, "&#250", "ú");
        replace_inplace(string, "&#251", "û");
        replace_inplace(string, "&#252", "ü");
        replace_inplace(string, "&#253", "ý");
        replace_inplace(string, "&#254", "þ");
        replace_inplace(string, "&#255", "ÿ");
        replace_inplace(string, "&#338", "Œ");
        replace_inplace(string, "&#339", "œ");
        replace_inplace(string, "&#352", "Š");
        replace_inplace(string, "&#353", "š");
        replace_inplace(string, "&#376", "Ÿ");
        replace_inplace(string, "&#402", "ƒ");
        replace_inplace(string, "&#710", "ˆ");
        replace_inplace(string, "&#732", "˜");
        replace_inplace(string, "&#913", "Α");
        replace_inplace(string, "&#914", "Β");
        replace_inplace(string, "&#915", "Γ");
        replace_inplace(string, "&#916", "Δ");
        replace_inplace(string, "&#917", "Ε");
        replace_inplace(string, "&#918", "Ζ");
        replace_inplace(string, "&#919", "Η");
        replace_inplace(string, "&#920", "Θ");
        replace_inplace(string, "&#921", "Ι");
        replace_inplace(string, "&#922", "Κ");
        replace_inplace(string, "&#923", "Λ");
        replace_inplace(string, "&#924", "Μ");
        replace_inplace(string, "&#925", "Ν");
        replace_inplace(string, "&#926", "Ξ");
        replace_inplace(string, "&#927", "Ο");
        replace_inplace(string, "&#928", "Π");
        replace_inplace(string, "&#929", "Ρ");
        replace_inplace(string, "&#931", "Σ");
        replace_inplace(string, "&#932", "Τ");
        replace_inplace(string, "&#933", "Υ");
        replace_inplace(string, "&#934", "Φ");
        replace_inplace(string, "&#935", "Χ");
        replace_inplace(string, "&#936", "Ψ");
        replace_inplace(string, "&#937", "Ω");
        replace_inplace(string, "&#945", "α");
        replace_inplace(string, "&#946", "β");
        replace_inplace(string, "&#947", "γ");
        replace_inplace(string, "&#948", "δ");
        replace_inplace(string, "&#949", "ε");
        replace_inplace(string, "&#950", "ζ");
        replace_inplace(string, "&#951", "η");
        replace_inplace(string, "&#952", "θ");
        replace_inplace(string, "&#953", "ι");
        replace_inplace(string, "&#954", "κ");
        replace_inplace(string, "&#955", "λ");
        replace_inplace(string, "&#956", "μ");
        replace_inplace(string, "&#957", "ν");
        replace_inplace(string, "&#958", "ξ");
        replace_inplace(string, "&#959", "ο");
        replace_inplace(string, "&#960", "π");
        replace_inplace(string, "&#961", "ρ");
        replace_inplace(string, "&#962", "ς");
        replace_inplace(string, "&#963", "σ");
        replace_inplace(string, "&#964", "τ");
        replace_inplace(string, "&#965", "υ");
        replace_inplace(string, "&#966", "φ");
        replace_inplace(string, "&#967", "χ");
        replace_inplace(string, "&#968", "ψ");
        replace_inplace(string, "&#969", "ω");
        replace_inplace(string, "&#977", "ϑ");
        replace_inplace(string, "&#978", "ϒ");
        replace_inplace(string, "&#982", "ϖ");
        replace_inplace(string, "&#8194", " ");
        replace_inplace(string, "&#8195", " ");
        replace_inplace(string, "&#8201", " ");
        replace_inplace(string, "&#8204", "‌");
        replace_inplace(string, "&#8205", "‍");
        replace_inplace(string, "&#8206", "‎");
        replace_inplace(string, "&#8207", "‏");
        replace_inplace(string, "&#8211", "–");
        replace_inplace(string, "&#8212", "—");
        replace_inplace(string, "&#8216", "‘");
        replace_inplace(string, "&#8217", "’");
        replace_inplace(string, "&#8218", "‚");
        replace_inplace(string, "&#8220", "“");
        replace_inplace(string, "&#8221", "”");
        replace_inplace(string, "&#8222", "„");
        replace_inplace(string, "&#8224", "†");
        replace_inplace(string, "&#8225", "‡");
        replace_inplace(string, "&#8226", "•");
        replace_inplace(string, "&#8230", "…");
        replace_inplace(string, "&#8240", "‰");
        replace_inplace(string, "&#8242", "′");
        replace_inplace(string, "&#8243", "″");
        replace_inplace(string, "&#8249", "‹");
        replace_inplace(string, "&#8250", "›");
        replace_inplace(string, "&#8254", "‾");
        replace_inplace(string, "&#8260", "⁄");
        replace_inplace(string, "&#8364", "€");
        replace_inplace(string, "&#8465", "ℑ");
        replace_inplace(string, "&#8472", "℘");
        replace_inplace(string, "&#8476", "ℜ");
        replace_inplace(string, "&#8482", "™");
        replace_inplace(string, "&#8501", "ℵ");
        replace_inplace(string, "&#8592", "←");
        replace_inplace(string, "&#8593", "↑");
        replace_inplace(string, "&#8594", "→");
        replace_inplace(string, "&#8595", "↓");
        replace_inplace(string, "&#8596", "↔");
        replace_inplace(string, "&#8629", "↵");
        replace_inplace(string, "&#8656", "⇐");
        replace_inplace(string, "&#8657", "⇑");
        replace_inplace(string, "&#8658", "⇒");
        replace_inplace(string, "&#8659", "⇓");
        replace_inplace(string, "&#8660", "⇔");
        replace_inplace(string, "&#8704", "∀");
        replace_inplace(string, "&#8706", "∂");
        replace_inplace(string, "&#8707", "∃");
        replace_inplace(string, "&#8709", "∅");
        replace_inplace(string, "&#8711", "∇");
        replace_inplace(string, "&#8712", "∈");
        replace_inplace(string, "&#8713", "∉");
        replace_inplace(string, "&#8715", "∋");
        replace_inplace(string, "&#8719", "∏");
        replace_inplace(string, "&#8721", "∑");
        replace_inplace(string, "&#8722", "−");
        replace_inplace(string, "&#8727", "∗");
        replace_inplace(string, "&#8730", "√");
        replace_inplace(string, "&#8733", "∝");
        replace_inplace(string, "&#8734", "∞");
        replace_inplace(string, "&#8736", "∠");
        replace_inplace(string, "&#8743", "∧");
        replace_inplace(string, "&#8744", "∨");
        replace_inplace(string, "&#8745", "∩");
        replace_inplace(string, "&#8746", "∪");
        replace_inplace(string, "&#8747", "∫");
        replace_inplace(string, "&#8756", "∴");
        replace_inplace(string, "&#8764", "∼");
        replace_inplace(string, "&#8773", "≅");
        replace_inplace(string, "&#8776", "≈");
        replace_inplace(string, "&#8800", "≠");
        replace_inplace(string, "&#8801", "≡");
        replace_inplace(string, "&#8804", "≤");
        replace_inplace(string, "&#8805", "≥");
        replace_inplace(string, "&#8834", "⊂");
        replace_inplace(string, "&#8835", "⊃");
        replace_inplace(string, "&#8836", "⊄");
        replace_inplace(string, "&#8838", "⊆");
        replace_inplace(string, "&#8839", "⊇");
        replace_inplace(string, "&#8853", "⊕");
        replace_inplace(string, "&#8855", "⊗");
        replace_inplace(string, "&#8869", "⊥");
        replace_inplace(string, "&#8901", "⋅");
        replace_inplace(string, "&#8968", "⌈");
        replace_inplace(string, "&#8969", "⌉");
        replace_inplace(string, "&#8970", "⌊");
        replace_inplace(string, "&#8971", "⌋");
        replace_inplace(string, "&#9674", "◊");
        replace_inplace(string, "&#9824", "♠");
        replace_inplace(string, "&#9827", "♣");
        replace_inplace(string, "&#9829", "♥");
        replace_inplace(string, "&#9830", "♦");
        replace_inplace(string, "&#10216", "⟨");
        replace_inplace(string, "&#10217", "⟩");
    }
}

void destroy_strings(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        free(va_arg(args, char *));
    }
}


multi_tok_t init() {
    return NULL;
}

char *multi_tok(char *input, char *delimiter) {
    if (input != NULL)
        s = input;

    if (s == NULL)
        return s;

    char *end = strstr(s, delimiter);
    if (end == NULL) {
        char *temp = s;
        s = NULL;
        return temp;
    }

    char *temp = s;

    *end = '\0';
    s = end + strlen(delimiter);
    return temp;
}


FILE *open_file(char *filename, char *mode) {
    FILE *file = fopen(filename, mode);

    if (file == NULL) {
        printf("Error: Could not open file '%s'.\n", filename);
        exit(1);
    }

    return file;
}

void close_file(FILE *file, char *filename) {
    if (fclose(file) != 0) {
        printf("Error: Could not close file '%s'.\n", filename);
        exit(1);
    }
}



// This is dependent on the ARRAY.H library
#ifndef ARRAY_H
#include "./array.h"
#endif

#ifdef ARRAY_H

ARRAY tokenize(char *string, char *delimiter);

char *join_tokens(ARRAY array, char *delimiter);

ARRAY tokenize(char *string, char *delimiter) {
    ARRAY array = array_create(1);

    char *copy = remove_newline(string);
    char *token = multi_tok(copy, delimiter);

    while (token != NULL) {
        array_add(array, (void *) create_string(token));
        token = multi_tok(NULL, delimiter);
    }

    destroy_strings(1, copy);

    return array;
}

char *join_tokens(ARRAY array, char *delimiter) {
    char *str = create_string("");

    for (int i = 0; i < array->size; i++) {
        str = concat_strings(3, str, (char *) array->data[i], delimiter);
    }

    str[strlen(str) - strlen(delimiter)] = '\0';

    return str;
}


#endif


#endif