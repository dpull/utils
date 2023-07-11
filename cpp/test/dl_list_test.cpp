#include "dl_list.h"
#include "gtest/gtest.h"

struct pawn {
    int uid;
    int x;
    int y;
    int z;

    dl_list_node node;
};

TEST(dl_list, sample)
{
    dl_list_node list;
    int ret;

    ret = dl_list_init(&list);
    ASSERT_TRUE(ret);

    for (int i = 0; i < 10; i++) {
        auto p = new pawn;
        p->uid = i;
        p->x = i * 10;
        p->y = i * 100;
        p->z = i * 1000;

        ret = dl_list_push_front(&list, &p->node);
        ASSERT_TRUE(ret);
    }

    for (int i = 0; i < 10; i++) {
        auto p = new pawn;
        p->uid = i + 10;
        p->x = i * 10;
        p->y = i * 100;
        p->z = i * 1000;

        ret = dl_list_push_back(&list, &p->node);
        ASSERT_TRUE(ret);
    }

    if (!dl_list_empty(&list)) {
        auto n = dl_list_pop_front(&list);
        auto p = CONTAINING_RECORD(n, pawn, node);
        ASSERT_EQ(p->uid, 9);
        delete p;
    }

    auto* select = list.next;
    while (select != &list) {
        auto* p = CONTAINING_RECORD(select, pawn, node);
        select = select->next;

        if (p->uid == 10) {
            ret = dl_list_erase(&p->node);
            ASSERT_TRUE(ret);
            delete p;
        }
    }

    while (!dl_list_empty(&list)) {
        auto n = dl_list_pop_front(&list);
        auto p = CONTAINING_RECORD(n, pawn, node);
        delete p;
    }
}
