class Medium
{
public:
    Medium(string mediumName)
    {
        mName = mediumName;
    }
    virtual string connect() const = 0;
    virtual string id() const = 0;
    virtual string transmit(string message) const
    {
        return "text: " + message;


    }
   virtual ~Medium()
    {
    }
private:
    string mName;

};

class XAccount : public Medium
{
public:
    XAccount(string handle) : mHandle(handle), Medium("X account")
    {

    }
    virtual string connect() const
    {
        return "Post to X";
    }

    virtual string id() const
    {
        return mHandle;
    }

    ~XAccount()
    {
        cout << "Destroying the X account "  << mHandle  << "." << endl;
    }

private:
    string mHandle;

};

class EmailAccount : public Medium
{
public:
    EmailAccount(string email) : mEmailAddress(email), Medium("email account")
    {

    }

    virtual string connect() const
    {
        return "Email";
    }

    virtual string id() const
    {
        return mEmailAddress;
    }

    ~EmailAccount()
    {
        cout << "Destroying the email account " << mEmailAddress << "." << endl;
    }
private:
    string mEmailAddress;
};

class Phone : public Medium
{

public:
    Phone(string phoneNumber, CallType calltype) : mPhoneNumber(phoneNumber), mCallType(calltype), Medium("phone")
    {
    }

    virtual string connect() const
    {
        return "Call";
    }

    virtual string transmit(string message) const
    {
        if (mCallType == VOICE)
        {
            return "voice: " + message;
        }
        else
        {
            return "text: " + message; // does this count as an identical implementation of Medium's default transmit function?
        }
    }

    virtual string id() const
    {
        return mPhoneNumber;
    }

    ~Phone()
    {
        cout << "Destroying the phone " << mPhoneNumber << "." << endl;
    }

private:
    string mPhoneNumber;
    CallType mCallType;
};
