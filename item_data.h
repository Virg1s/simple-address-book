#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#define NAME_MAX 50
#define SURNAME_MAX 50
#define EMAIL_MAX 100
#define PHONE_MAX 30

struct item_data {
        char name[NAME_MAX];
        char surname[SURNAME_MAX];
        char email[EMAIL_MAX];
        char phone[PHONE_MAX];
};


#endif
