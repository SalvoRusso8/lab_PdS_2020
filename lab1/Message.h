//
// Created by Salvo-fisso on 21/04/2020.
//

#ifndef LAB1_MESSAGE_H
#define LAB1_MESSAGE_H


class Message {
    long id;
    char *data;
    int size;

public:
    Message(int n);

    Message();

    Message(const Message& m);

    Message( Message&& m);

    void setId(long id);

    void setData(char *data);

    void setSize(int size);

    long getId() const;

    char *getData() const;

    int getSize() const;

    Message& operator=( const Message &m);

    Message& operator=( Message&& m);

    ~Message();

private:
    static long id_univ;
    char* mkMessage(int n);


};

std::ostream& operator<<(std::ostream& out, const Message& m);


#endif //LAB1_MESSAGE_H
