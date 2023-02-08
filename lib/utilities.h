#ifndef UTILITIES_H
#define UTILITIES_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Declarations

int count_digits(long long n);

char *string_create(char *string);
char *string_copy(char *string);
char *string_lowercase(char *string, bool inplace);
char *string_uppercase(char *string, bool inplace);
char *string_trim(char *string, bool inplace);
char *string_concat(bool inplace, int count, ...);
char *string_truncate(char *string, char *start, char *end, bool inplace);
void string_destroy(char *string);
void strings_destroy(int count, ...);

// Replace operations
#define replace_all(string, find, replace) _Generic(find, \
    int: replace_all_char,                                \
    char *: replace_all_string                            \
) (string, find, replace)

char *replace_all_char(char *string, char find, char replace);
char *replace_all_string(char *string, char *find, char *replace);
// End of replace operations

// Remove operations
#define remove_all(string, find) _Generic(find, \
    int: remove_all_char,                       \
    char *: remove_all_string                   \
) (string, find)

char *remove_all_char(char *string, char find);
char *remove_all_string(char *string, char *find);
// End of remove operations

char *html_unicode_converter(char *string);
char *deci_unicode_converter(char *string);
char *string_to_url(char *string);

// Multi-tokenizer
typedef char *multi_tok_t;

multi_tok_t init();
char *multi_tok(char *input, char *delimiter);
// End of multi-tokenizer

FILE *open_file(char *filename, char *mode);
void close_file(FILE *file, char *filename);

// Implementations

int count_digits(long long n) {
    if (n / 10 == 0)
        return 1;

    return 1 + count_digits(n / 10);
}



char *string_create(char *string) {
    if (string == NULL) { return NULL; }

    char *str = (char *) malloc(strlen(string) + 1);
    strcpy(str, string);
    return str;
}

char *string_copy(char *string) {
    if (string == NULL) { return NULL; }

    char *str = (char *) malloc(strlen(string) + 1);
    strcpy(str, string);
    return str;
}

char *string_lowercase(char *string, bool inplace) {
    if (string == NULL) { return NULL; }

    char *str;
    if (inplace) { str = string; }
    else { str = string_copy(string); }

    for (int i = 0; i < (int) strlen(str); i++) {
        str[i] = tolower(str[i]);
    }

    return str;
}

char *string_uppercase(char *string, bool inplace) {
    if (string == NULL) { return NULL; }

    char *str;
    if (inplace) { str = string; }
    else { str = string_copy(string); }
    
    for (int i = 0; i < (int) strlen(str); i++) {
        str[i] = toupper(str[i]);
    }

    return str;
}

char *string_trim(char *string, bool inplace) {
    if (string == NULL) { return NULL; }

    char *str;

    if (inplace) { str = string; }
    else { str = string_copy(string); }

    int i = 0;
    while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == '\r') { i++; }

    int j = strlen(str) - 1;
    while (str[j] == ' ' || str[j] == '\n' || str[j] == '\t' || str[j] == '\r') { j--; }

    memmove(str, str + i, j - i + 1);
    str[j - i + 1] = '\0';

    return str;
}

char *string_concat(bool inplace, int count, ...) {
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
    
    if (inplace) {
        va_start(args, count);
        char *pointer = va_arg(args, char *);
        free(pointer);
        pointer = str;
    }

    return str;
}

char *string_truncate(char *string, char *start, char *end, bool inplace) {
    if (string == NULL) { return NULL; }

    char *start_index = strstr(string, start);
    if (start_index == NULL) { return (inplace) ? string : NULL; }

    char *end_index = strstr(start_index + strlen(start), end);
    if (end_index == NULL) { return (inplace) ? string : NULL; }

    int start_offset = strlen(start);
    // int end_offset = strlen(end);

    int length = end_index - start_index - start_offset;
    char *truncated = (char *) malloc(length + 1);
    strncpy(truncated, start_index + start_offset, length);
    truncated[length] = '\0';

    if (inplace) { free(string); }

    return truncated;
}

void string_destroy(char *string) {
    if (string != NULL)
        free(string);
}

void strings_destroy(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        free(va_arg(args, char *));
    }
}



char *replace_all_char(char *string, char find, char replace) {
    if (string == NULL) { return NULL; }

    for (int i = 0; i < (int) strlen(string); i++) {
        if (string[i] == find) {
            string[i] = replace;
        }
    }

    return string;
}

char *replace_all_string(char *string, char *find, char *replace) {
    if (string == NULL) { return NULL; }

    char *temp = NULL;
    int last_index = 0;

    while ((temp = strstr(string + last_index, find)) != NULL) {
        int index = temp - string;
        int length = strlen(string) - strlen(find) + strlen(replace) + 1;

        char *new = (char *) malloc(sizeof(char) * length);
        strncpy(new, string, index);
        strcpy(new + index, replace);
        strcpy(new + index + strlen(replace), string + index + strlen(find));

        free(string);
        string = new;
    }

    return string;
}



char *remove_all_char(char *string, char find) {
    if (string == NULL) { return NULL; }

    char *temp = NULL;

    while ((temp = strchr(string, find)) != NULL) {
        int index = temp - string;
        int length = strlen(string);

        char *new = (char *) malloc(sizeof(char) * length);
        strncpy(new, string, index);
        strcpy(new + index, string + index + 1);

        free(string);
        string = new;
    }

    return string;
}

char *remove_all_string(char *string, char *find) {
    if (string == NULL) { return NULL; }

    char *temp = NULL;

    while ((temp = strstr(string, find)) != NULL) {
        int index = temp - string;
        int length = strlen(string) - strlen(find) + 1;

        char *new = (char *) malloc(sizeof(char) * length);
        strncpy(new, string, index);
        strcpy(new + index, string + index + strlen(find));

        free(string);
        string = new;
    }

    return string;
}



char *html_unicode_converter(char *string) {
    if (string == NULL) { return NULL; }

    string = replace_all(string, "&quot;", "\"");
    string = replace_all(string, "&amp;", "&");
    string = replace_all(string, "&apos;", "'");
    string = replace_all(string, "&lt;", "<");
    string = replace_all(string, "&gt;", ">");
    string = replace_all(string, "&nbsp;", " ");
    string = replace_all(string, "&iexcl;", "¡");
    string = replace_all(string, "&cent;", "¢");
    string = replace_all(string, "&pound;", "£");
    string = replace_all(string, "&curren;", "¤");
    string = replace_all(string, "&yen;", "¥");
    string = replace_all(string, "&brvbar;", "¦");
    string = replace_all(string, "&sect;", "§");
    string = replace_all(string, "&uml;", "¨");
    string = replace_all(string, "&copy;", "©");
    string = replace_all(string, "&ordf;", "ª");
    string = replace_all(string, "&laquo;", "«");
    string = replace_all(string, "&not;", "¬");
    string = replace_all(string, "&shy;", "­");
    string = replace_all(string, "&reg;", "®");
    string = replace_all(string, "&macr;", "¯");
    string = replace_all(string, "&deg;", "°");
    string = replace_all(string, "&plusmn;", "±");
    string = replace_all(string, "&sup2;", "²");
    string = replace_all(string, "&sup3;", "³");
    string = replace_all(string, "&acute;", "´");
    string = replace_all(string, "&micro;", "µ");
    string = replace_all(string, "&para;", "¶");
    string = replace_all(string, "&middot;", "·");
    string = replace_all(string, "&cedil;", "¸");
    string = replace_all(string, "&sup1;", "¹");
    string = replace_all(string, "&ordm;", "º");
    string = replace_all(string, "&raquo;", "»");
    string = replace_all(string, "&frac14;", "¼");
    string = replace_all(string, "&frac12;", "½");
    string = replace_all(string, "&frac34;", "¾");
    string = replace_all(string, "&iquest;", "¿");
    string = replace_all(string, "&Agrave;", "À");
    string = replace_all(string, "&Aacute;", "Á");
    string = replace_all(string, "&Acirc;", "Â");
    string = replace_all(string, "&Atilde;", "Ã");
    string = replace_all(string, "&Auml;", "Ä");
    string = replace_all(string, "&Aring;", "Å");
    string = replace_all(string, "&AElig;", "Æ");
    string = replace_all(string, "&Ccedil;", "Ç");
    string = replace_all(string, "&Egrave;", "È");
    string = replace_all(string, "&Eacute;", "É");
    string = replace_all(string, "&Ecirc;", "Ê");
    string = replace_all(string, "&Euml;", "Ë");
    string = replace_all(string, "&Igrave;", "Ì");
    string = replace_all(string, "&Iacute;", "Í");
    string = replace_all(string, "&Icirc;", "Î");
    string = replace_all(string, "&Iuml;", "Ï");
    string = replace_all(string, "&ETH;", "Ð");
    string = replace_all(string, "&Ntilde;", "Ñ");
    string = replace_all(string, "&Ograve;", "Ò");
    string = replace_all(string, "&Oacute;", "Ó");
    string = replace_all(string, "&Ocirc;", "Ô");
    string = replace_all(string, "&Otilde;", "Õ");
    string = replace_all(string, "&Ouml;", "Ö");
    string = replace_all(string, "&times;", "×");
    string = replace_all(string, "&Oslash;", "Ø");
    string = replace_all(string, "&Ugrave;", "Ù");
    string = replace_all(string, "&Uacute;", "Ú");
    string = replace_all(string, "&Ucirc;", "Û");
    string = replace_all(string, "&Uuml;", "Ü");
    string = replace_all(string, "&Yacute;", "Ý");
    string = replace_all(string, "&THORN;", "Þ");
    string = replace_all(string, "&szlig;", "ß");
    string = replace_all(string, "&agrave;", "à");
    string = replace_all(string, "&aacute;", "á");
    string = replace_all(string, "&acirc;", "â");
    string = replace_all(string, "&atilde;", "ã");
    string = replace_all(string, "&auml;", "ä");
    string = replace_all(string, "&aring;", "å");
    string = replace_all(string, "&aelig;", "æ");
    string = replace_all(string, "&ccedil;", "ç");
    string = replace_all(string, "&egrave;", "è");
    string = replace_all(string, "&eacute;", "é");
    string = replace_all(string, "&ecirc;", "ê");
    string = replace_all(string, "&euml;", "ë");
    string = replace_all(string, "&igrave;", "ì");
    string = replace_all(string, "&iacute;", "í");
    string = replace_all(string, "&icirc;", "î");
    string = replace_all(string, "&iuml;", "ï");
    string = replace_all(string, "&eth;", "ð");
    string = replace_all(string, "&ntilde;", "ñ");
    string = replace_all(string, "&ograve;", "ò");
    string = replace_all(string, "&oacute;", "ó");
    string = replace_all(string, "&ocirc;", "ô");
    string = replace_all(string, "&otilde;", "õ");
    string = replace_all(string, "&ouml;", "ö");
    string = replace_all(string, "&divide;", "÷");
    string = replace_all(string, "&oslash;", "ø");
    string = replace_all(string, "&ugrave;", "ù");
    string = replace_all(string, "&uacute;", "ú");
    string = replace_all(string, "&ucirc;", "û");
    string = replace_all(string, "&uuml;", "ü");
    string = replace_all(string, "&yacute;", "ý");
    string = replace_all(string, "&thorn;", "þ");
    string = replace_all(string, "&yuml;", "ÿ");
    string = replace_all(string, "&OElig;", "Œ");
    string = replace_all(string, "&oelig;", "œ");
    string = replace_all(string, "&Scaron;", "Š");
    string = replace_all(string, "&scaron;", "š");
    string = replace_all(string, "&Yuml;", "Ÿ");
    string = replace_all(string, "&fnof;", "ƒ");
    string = replace_all(string, "&circ;", "ˆ");
    string = replace_all(string, "&tilde;", "˜");
    string = replace_all(string, "&Alpha;", "Α");
    string = replace_all(string, "&Beta;", "Β");
    string = replace_all(string, "&Gamma;", "Γ");
    string = replace_all(string, "&Delta;", "Δ");
    string = replace_all(string, "&Epsilon;", "Ε");
    string = replace_all(string, "&Zeta;", "Ζ");
    string = replace_all(string, "&Eta;", "Η");
    string = replace_all(string, "&Theta;", "Θ");
    string = replace_all(string, "&Iota;", "Ι");
    string = replace_all(string, "&Kappa;", "Κ");
    string = replace_all(string, "&Lambda;", "Λ");
    string = replace_all(string, "&Mu;", "Μ");
    string = replace_all(string, "&Nu;", "Ν");
    string = replace_all(string, "&Xi;", "Ξ");
    string = replace_all(string, "&Omicron;", "Ο");
    string = replace_all(string, "&Pi;", "Π");
    string = replace_all(string, "&Rho;", "Ρ");
    string = replace_all(string, "&Sigma;", "Σ");
    string = replace_all(string, "&Tau;", "Τ");
    string = replace_all(string, "&Upsilon;", "Υ");
    string = replace_all(string, "&Phi;", "Φ");
    string = replace_all(string, "&Chi;", "Χ");
    string = replace_all(string, "&Psi;", "Ψ");
    string = replace_all(string, "&Omega;", "Ω");
    string = replace_all(string, "&alpha;", "α");
    string = replace_all(string, "&beta;", "β");
    string = replace_all(string, "&gamma;", "γ");
    string = replace_all(string, "&delta;", "δ");
    string = replace_all(string, "&epsilon;", "ε");
    string = replace_all(string, "&zeta;", "ζ");
    string = replace_all(string, "&eta;", "η");
    string = replace_all(string, "&theta;", "θ");
    string = replace_all(string, "&iota;", "ι");
    string = replace_all(string, "&kappa;", "κ");
    string = replace_all(string, "&lambda;", "λ");
    string = replace_all(string, "&mu;", "μ");
    string = replace_all(string, "&nu;", "ν");
    string = replace_all(string, "&xi;", "ξ");
    string = replace_all(string, "&omicron;", "ο");
    string = replace_all(string, "&pi;", "π");
    string = replace_all(string, "&rho;", "ρ");
    string = replace_all(string, "&sigmaf;", "ς");
    string = replace_all(string, "&sigma;", "σ");
    string = replace_all(string, "&tau;", "τ");
    string = replace_all(string, "&upsilon;", "υ");
    string = replace_all(string, "&phi;", "φ");
    string = replace_all(string, "&chi;", "χ");
    string = replace_all(string, "&psi;", "ψ");
    string = replace_all(string, "&omega;", "ω");
    string = replace_all(string, "&thetasym;", "ϑ");
    string = replace_all(string, "&upsih;", "ϒ");
    string = replace_all(string, "&piv;", "ϖ");
    string = replace_all(string, "&ensp;", " ");
    string = replace_all(string, "&emsp;", " ");
    string = replace_all(string, "&thinsp;", " ");
    string = replace_all(string, "&zwnj;", "‌");
    string = replace_all(string, "&zwj;", "‍");
    string = replace_all(string, "&lrm;", "‎");
    string = replace_all(string, "&rlm;", "‏");
    string = replace_all(string, "&ndash;", "–");
    string = replace_all(string, "&mdash;", "—");
    string = replace_all(string, "&lsquo;", "‘");
    string = replace_all(string, "&rsquo;", "’");
    string = replace_all(string, "&sbquo;", "‚");
    string = replace_all(string, "&ldquo;", "“");
    string = replace_all(string, "&rdquo;", "”");
    string = replace_all(string, "&bdquo;", "„");
    string = replace_all(string, "&dagger;", "†");
    string = replace_all(string, "&Dagger;", "‡");
    string = replace_all(string, "&bull;", "•");
    string = replace_all(string, "&hellip;", "…");
    string = replace_all(string, "&permil;", "‰");
    string = replace_all(string, "&prime;", "′");
    string = replace_all(string, "&Prime;", "″");
    string = replace_all(string, "&lsaquo;", "‹");
    string = replace_all(string, "&rsaquo;", "›");
    string = replace_all(string, "&oline;", "‾");
    string = replace_all(string, "&frasl;", "⁄");
    string = replace_all(string, "&euro;", "€");
    string = replace_all(string, "&image;", "ℑ");
    string = replace_all(string, "&weierp;", "℘");
    string = replace_all(string, "&real;", "ℜ");
    string = replace_all(string, "&trade;", "™");
    string = replace_all(string, "&alefsym;", "ℵ");
    string = replace_all(string, "&larr;", "←");
    string = replace_all(string, "&uarr;", "↑");
    string = replace_all(string, "&rarr;", "→");
    string = replace_all(string, "&darr;", "↓");
    string = replace_all(string, "&harr;", "↔");
    string = replace_all(string, "&crarr;", "↵");
    string = replace_all(string, "&lArr;", "⇐");
    string = replace_all(string, "&uArr;", "⇑");
    string = replace_all(string, "&rArr;", "⇒");
    string = replace_all(string, "&dArr;", "⇓");
    string = replace_all(string, "&hArr;", "⇔");
    string = replace_all(string, "&forall;", "∀");
    string = replace_all(string, "&part;", "∂");
    string = replace_all(string, "&exist;", "∃");
    string = replace_all(string, "&empty;", "∅");
    string = replace_all(string, "&nabla;", "∇");
    string = replace_all(string, "&isin;", "∈");
    string = replace_all(string, "&notin;", "∉");
    string = replace_all(string, "&ni;", "∋");
    string = replace_all(string, "&prod;", "∏");
    string = replace_all(string, "&sum;", "∑");
    string = replace_all(string, "&minus;", "−");
    string = replace_all(string, "&lowast;", "∗");
    string = replace_all(string, "&radic;", "√");
    string = replace_all(string, "&prop;", "∝");
    string = replace_all(string, "&infin;", "∞");
    string = replace_all(string, "&ang;", "∠");
    string = replace_all(string, "&and;", "∧");
    string = replace_all(string, "&or;", "∨");
    string = replace_all(string, "&cap;", "∩");
    string = replace_all(string, "&cup;", "∪");
    string = replace_all(string, "&int;", "∫");
    string = replace_all(string, "&there4;", "∴");
    string = replace_all(string, "&sim;", "∼");
    string = replace_all(string, "&cong;", "≅");
    string = replace_all(string, "&asymp;", "≈");
    string = replace_all(string, "&ne;", "≠");
    string = replace_all(string, "&equiv;", "≡");
    string = replace_all(string, "&le;", "≤");
    string = replace_all(string, "&ge;", "≥");
    string = replace_all(string, "&sub;", "⊂");
    string = replace_all(string, "&sup;", "⊃");
    string = replace_all(string, "&nsub;", "⊄");
    string = replace_all(string, "&sube;", "⊆");
    string = replace_all(string, "&supe;", "⊇");
    string = replace_all(string, "&oplus;", "⊕");
    string = replace_all(string, "&otimes;", "⊗");
    string = replace_all(string, "&perp;", "⊥");
    string = replace_all(string, "&sdot;", "⋅");
    string = replace_all(string, "&lceil;", "⌈");
    string = replace_all(string, "&rceil;", "⌉");
    string = replace_all(string, "&lfloor;", "⌊");
    string = replace_all(string, "&rfloor;", "⌋");
    string = replace_all(string, "&loz;", "◊");
    string = replace_all(string, "&spades;", "♠");
    string = replace_all(string, "&clubs;", "♣");
    string = replace_all(string, "&hearts;", "♥");
    string = replace_all(string, "&diams;", "♦");
    string = replace_all(string, "&lang;", "⟨");
    string = replace_all(string, "&rang;", "⟩");

    return string;
}

char *html_remove_italic(char *string) {
    if (string == NULL) { return NULL; }

    string = replace_all(string, "<i>", "");
    string = replace_all(string, "</i>", "");

    return string;
}

char *html_remove_bold(char *string) {
    if (string == NULL) { return NULL; }

    string = replace_all(string, "<b>", "");
    string = replace_all(string, "</b>", "");

    return string;
}

char *deci_unicode_converter(char *string) {
    if (string == NULL) { return NULL; }

    char *pointer = string;

    while ((pointer = strstr(pointer, "&#")) != NULL) {
        // Remove the left padding zeros
        while (*(pointer + 2) == '0') {
            for (int i = 2; i < (int) strlen(pointer); i++) {
                pointer[i] = pointer[i + 1];
            }
        }

        // Remove the semicolon
        pointer = strstr(pointer, ";");
        for (int i = 0; i < (int) strlen(pointer); i++) {
            pointer[i] = pointer[i + 1];
        }
    }

    string = replace_all(string, "&#34", "\"");
    string = replace_all(string, "&#38", "&");
    string = replace_all(string, "&#39", "'");
    string = replace_all(string, "&#60", "<");
    string = replace_all(string, "&#62", ">");
    string = replace_all(string, "&#160", " ");
    string = replace_all(string, "&#161", "¡");
    string = replace_all(string, "&#162", "¢");
    string = replace_all(string, "&#163", "£");
    string = replace_all(string, "&#164", "¤");
    string = replace_all(string, "&#165", "¥");
    string = replace_all(string, "&#166", "¦");
    string = replace_all(string, "&#167", "§");
    string = replace_all(string, "&#168", "¨");
    string = replace_all(string, "&#169", "©");
    string = replace_all(string, "&#170", "ª");
    string = replace_all(string, "&#171", "«");
    string = replace_all(string, "&#172", "¬");
    string = replace_all(string, "&#173", "­");
    string = replace_all(string, "&#174", "®");
    string = replace_all(string, "&#175", "¯");
    string = replace_all(string, "&#176", "°");
    string = replace_all(string, "&#177", "±");
    string = replace_all(string, "&#178", "²");
    string = replace_all(string, "&#179", "³");
    string = replace_all(string, "&#180", "´");
    string = replace_all(string, "&#181", "µ");
    string = replace_all(string, "&#182", "¶");
    string = replace_all(string, "&#183", "·");
    string = replace_all(string, "&#184", "¸");
    string = replace_all(string, "&#185", "¹");
    string = replace_all(string, "&#186", "º");
    string = replace_all(string, "&#187", "»");
    string = replace_all(string, "&#188", "¼");
    string = replace_all(string, "&#189", "½");
    string = replace_all(string, "&#190", "¾");
    string = replace_all(string, "&#191", "¿");
    string = replace_all(string, "&#192", "À");
    string = replace_all(string, "&#193", "Á");
    string = replace_all(string, "&#194", "Â");
    string = replace_all(string, "&#195", "Ã");
    string = replace_all(string, "&#196", "Ä");
    string = replace_all(string, "&#197", "Å");
    string = replace_all(string, "&#198", "Æ");
    string = replace_all(string, "&#199", "Ç");
    string = replace_all(string, "&#200", "È");
    string = replace_all(string, "&#201", "É");
    string = replace_all(string, "&#202", "Ê");
    string = replace_all(string, "&#203", "Ë");
    string = replace_all(string, "&#204", "Ì");
    string = replace_all(string, "&#205", "Í");
    string = replace_all(string, "&#206", "Î");
    string = replace_all(string, "&#207", "Ï");
    string = replace_all(string, "&#208", "Ð");
    string = replace_all(string, "&#209", "Ñ");
    string = replace_all(string, "&#210", "Ò");
    string = replace_all(string, "&#211", "Ó");
    string = replace_all(string, "&#212", "Ô");
    string = replace_all(string, "&#213", "Õ");
    string = replace_all(string, "&#214", "Ö");
    string = replace_all(string, "&#215", "×");
    string = replace_all(string, "&#216", "Ø");
    string = replace_all(string, "&#217", "Ù");
    string = replace_all(string, "&#218", "Ú");
    string = replace_all(string, "&#219", "Û");
    string = replace_all(string, "&#220", "Ü");
    string = replace_all(string, "&#221", "Ý");
    string = replace_all(string, "&#222", "Þ");
    string = replace_all(string, "&#223", "ß");
    string = replace_all(string, "&#224", "à");
    string = replace_all(string, "&#225", "á");
    string = replace_all(string, "&#226", "â");
    string = replace_all(string, "&#227", "ã");
    string = replace_all(string, "&#228", "ä");
    string = replace_all(string, "&#229", "å");
    string = replace_all(string, "&#230", "æ");
    string = replace_all(string, "&#231", "ç");
    string = replace_all(string, "&#232", "è");
    string = replace_all(string, "&#233", "é");
    string = replace_all(string, "&#234", "ê");
    string = replace_all(string, "&#235", "ë");
    string = replace_all(string, "&#236", "ì");
    string = replace_all(string, "&#237", "í");
    string = replace_all(string, "&#238", "î");
    string = replace_all(string, "&#239", "ï");
    string = replace_all(string, "&#240", "ð");
    string = replace_all(string, "&#241", "ñ");
    string = replace_all(string, "&#242", "ò");
    string = replace_all(string, "&#243", "ó");
    string = replace_all(string, "&#244", "ô");
    string = replace_all(string, "&#245", "õ");
    string = replace_all(string, "&#246", "ö");
    string = replace_all(string, "&#247", "÷");
    string = replace_all(string, "&#248", "ø");
    string = replace_all(string, "&#249", "ù");
    string = replace_all(string, "&#250", "ú");
    string = replace_all(string, "&#251", "û");
    string = replace_all(string, "&#252", "ü");
    string = replace_all(string, "&#253", "ý");
    string = replace_all(string, "&#254", "þ");
    string = replace_all(string, "&#255", "ÿ");
    string = replace_all(string, "&#338", "Œ");
    string = replace_all(string, "&#339", "œ");
    string = replace_all(string, "&#352", "Š");
    string = replace_all(string, "&#353", "š");
    string = replace_all(string, "&#376", "Ÿ");
    string = replace_all(string, "&#402", "ƒ");
    string = replace_all(string, "&#710", "ˆ");
    string = replace_all(string, "&#732", "˜");
    string = replace_all(string, "&#913", "Α");
    string = replace_all(string, "&#914", "Β");
    string = replace_all(string, "&#915", "Γ");
    string = replace_all(string, "&#916", "Δ");
    string = replace_all(string, "&#917", "Ε");
    string = replace_all(string, "&#918", "Ζ");
    string = replace_all(string, "&#919", "Η");
    string = replace_all(string, "&#920", "Θ");
    string = replace_all(string, "&#921", "Ι");
    string = replace_all(string, "&#922", "Κ");
    string = replace_all(string, "&#923", "Λ");
    string = replace_all(string, "&#924", "Μ");
    string = replace_all(string, "&#925", "Ν");
    string = replace_all(string, "&#926", "Ξ");
    string = replace_all(string, "&#927", "Ο");
    string = replace_all(string, "&#928", "Π");
    string = replace_all(string, "&#929", "Ρ");
    string = replace_all(string, "&#931", "Σ");
    string = replace_all(string, "&#932", "Τ");
    string = replace_all(string, "&#933", "Υ");
    string = replace_all(string, "&#934", "Φ");
    string = replace_all(string, "&#935", "Χ");
    string = replace_all(string, "&#936", "Ψ");
    string = replace_all(string, "&#937", "Ω");
    string = replace_all(string, "&#945", "α");
    string = replace_all(string, "&#946", "β");
    string = replace_all(string, "&#947", "γ");
    string = replace_all(string, "&#948", "δ");
    string = replace_all(string, "&#949", "ε");
    string = replace_all(string, "&#950", "ζ");
    string = replace_all(string, "&#951", "η");
    string = replace_all(string, "&#952", "θ");
    string = replace_all(string, "&#953", "ι");
    string = replace_all(string, "&#954", "κ");
    string = replace_all(string, "&#955", "λ");
    string = replace_all(string, "&#956", "μ");
    string = replace_all(string, "&#957", "ν");
    string = replace_all(string, "&#958", "ξ");
    string = replace_all(string, "&#959", "ο");
    string = replace_all(string, "&#960", "π");
    string = replace_all(string, "&#961", "ρ");
    string = replace_all(string, "&#962", "ς");
    string = replace_all(string, "&#963", "σ");
    string = replace_all(string, "&#964", "τ");
    string = replace_all(string, "&#965", "υ");
    string = replace_all(string, "&#966", "φ");
    string = replace_all(string, "&#967", "χ");
    string = replace_all(string, "&#968", "ψ");
    string = replace_all(string, "&#969", "ω");
    string = replace_all(string, "&#977", "ϑ");
    string = replace_all(string, "&#978", "ϒ");
    string = replace_all(string, "&#982", "ϖ");
    string = replace_all(string, "&#8194", " ");
    string = replace_all(string, "&#8195", " ");
    string = replace_all(string, "&#8201", " ");
    string = replace_all(string, "&#8204", "‌");
    string = replace_all(string, "&#8205", "‍");
    string = replace_all(string, "&#8206", "‎");
    string = replace_all(string, "&#8207", "‏");
    string = replace_all(string, "&#8211", "–");
    string = replace_all(string, "&#8212", "—");
    string = replace_all(string, "&#8216", "‘");
    string = replace_all(string, "&#8217", "’");
    string = replace_all(string, "&#8218", "‚");
    string = replace_all(string, "&#8220", "“");
    string = replace_all(string, "&#8221", "”");
    string = replace_all(string, "&#8222", "„");
    string = replace_all(string, "&#8224", "†");
    string = replace_all(string, "&#8225", "‡");
    string = replace_all(string, "&#8226", "•");
    string = replace_all(string, "&#8230", "…");
    string = replace_all(string, "&#8240", "‰");
    string = replace_all(string, "&#8242", "′");
    string = replace_all(string, "&#8243", "″");
    string = replace_all(string, "&#8249", "‹");
    string = replace_all(string, "&#8250", "›");
    string = replace_all(string, "&#8254", "‾");
    string = replace_all(string, "&#8260", "⁄");
    string = replace_all(string, "&#8364", "€");
    string = replace_all(string, "&#8465", "ℑ");
    string = replace_all(string, "&#8472", "℘");
    string = replace_all(string, "&#8476", "ℜ");
    string = replace_all(string, "&#8482", "™");
    string = replace_all(string, "&#8501", "ℵ");
    string = replace_all(string, "&#8592", "←");
    string = replace_all(string, "&#8593", "↑");
    string = replace_all(string, "&#8594", "→");
    string = replace_all(string, "&#8595", "↓");
    string = replace_all(string, "&#8596", "↔");
    string = replace_all(string, "&#8629", "↵");
    string = replace_all(string, "&#8656", "⇐");
    string = replace_all(string, "&#8657", "⇑");
    string = replace_all(string, "&#8658", "⇒");
    string = replace_all(string, "&#8659", "⇓");
    string = replace_all(string, "&#8660", "⇔");
    string = replace_all(string, "&#8704", "∀");
    string = replace_all(string, "&#8706", "∂");
    string = replace_all(string, "&#8707", "∃");
    string = replace_all(string, "&#8709", "∅");
    string = replace_all(string, "&#8711", "∇");
    string = replace_all(string, "&#8712", "∈");
    string = replace_all(string, "&#8713", "∉");
    string = replace_all(string, "&#8715", "∋");
    string = replace_all(string, "&#8719", "∏");
    string = replace_all(string, "&#8721", "∑");
    string = replace_all(string, "&#8722", "−");
    string = replace_all(string, "&#8727", "∗");
    string = replace_all(string, "&#8730", "√");
    string = replace_all(string, "&#8733", "∝");
    string = replace_all(string, "&#8734", "∞");
    string = replace_all(string, "&#8736", "∠");
    string = replace_all(string, "&#8743", "∧");
    string = replace_all(string, "&#8744", "∨");
    string = replace_all(string, "&#8745", "∩");
    string = replace_all(string, "&#8746", "∪");
    string = replace_all(string, "&#8747", "∫");
    string = replace_all(string, "&#8756", "∴");
    string = replace_all(string, "&#8764", "∼");
    string = replace_all(string, "&#8773", "≅");
    string = replace_all(string, "&#8776", "≈");
    string = replace_all(string, "&#8800", "≠");
    string = replace_all(string, "&#8801", "≡");
    string = replace_all(string, "&#8804", "≤");
    string = replace_all(string, "&#8805", "≥");
    string = replace_all(string, "&#8834", "⊂");
    string = replace_all(string, "&#8835", "⊃");
    string = replace_all(string, "&#8836", "⊄");
    string = replace_all(string, "&#8838", "⊆");
    string = replace_all(string, "&#8839", "⊇");
    string = replace_all(string, "&#8853", "⊕");
    string = replace_all(string, "&#8855", "⊗");
    string = replace_all(string, "&#8869", "⊥");
    string = replace_all(string, "&#8901", "⋅");
    string = replace_all(string, "&#8968", "⌈");
    string = replace_all(string, "&#8969", "⌉");
    string = replace_all(string, "&#8970", "⌊");
    string = replace_all(string, "&#8971", "⌋");
    string = replace_all(string, "&#9674", "◊");
    string = replace_all(string, "&#9824", "♠");
    string = replace_all(string, "&#9827", "♣");
    string = replace_all(string, "&#9829", "♥");
    string = replace_all(string, "&#9830", "♦");
    string = replace_all(string, "&#10216", "⟨");
    string = replace_all(string, "&#10217", "⟩");

    return string;
}

char *string_to_url(char *string) {
    if (string == NULL) { return NULL; }

    string = string_trim(string, true);

    string = replace_all(string, "%",  "%25"); // Must be first

    string = replace_all(string, " ",  "%20");
    string = replace_all(string, "!",  "%21");
    string = replace_all(string, "\"", "%22");
    string = replace_all(string, "#",  "%23");
    string = replace_all(string, "$",  "%24");
    string = replace_all(string, "&",  "%26");
    string = replace_all(string, "'",  "%27");
    string = replace_all(string, "(",  "%28");
    string = replace_all(string, ")",  "%29");
    string = replace_all(string, "*",  "%2A");
    string = replace_all(string, "+",  "%2B");
    string = replace_all(string, ",",  "%2C");
    string = replace_all(string, "-",  "%2D");
    string = replace_all(string, ".",  "%2E");
    string = replace_all(string, "/",  "%2F");
    string = replace_all(string, ":",  "%3A");
    string = replace_all(string, ";",  "%3B");
    string = replace_all(string, "<",  "%3C");
    string = replace_all(string, "=",  "%3D");
    string = replace_all(string, ">",  "%3E");
    string = replace_all(string, "?",  "%3F");
    string = replace_all(string, "@",  "%40");
    string = replace_all(string, "[",  "%5B");
    string = replace_all(string, "\\", "%5C");
    string = replace_all(string, "]",  "%5D");
    string = replace_all(string, "^",  "%5E");
    string = replace_all(string, "_",  "%5F");
    string = replace_all(string, "`",  "%60");
    string = replace_all(string, "{",  "%7B");
    string = replace_all(string, "|",  "%7C");
    string = replace_all(string, "}",  "%7D");
    string = replace_all(string, "~",  "%7E");

    return string;
}



/* The following code is from the following source:
 * https://stackoverflow.com/questions/29788983/split-char-string-with-multi-character-delimiter-in-c
 * Written by: https://stackoverflow.com/users/179910/jerry-coffin
 */
multi_tok_t s = NULL;

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

// End of code from source

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

    char *token = multi_tok(string_trim(string, true), delimiter);

    while (token != NULL) {
        array_add(array, (void *) string_create(token));
        token = multi_tok(NULL, delimiter);
    }

    return array;
}

char *join_tokens(ARRAY array, char *delimiter) {
    char *str = string_create("");

    for (int i = 0; i < array->size; i++) {
        str = string_concat(false, 3, str, (char *) array->data[i], delimiter);
    }

    str[strlen(str) - strlen(delimiter)] = '\0';

    return str;
}


#endif // ARRAY_H

#endif // UTILITIES_H