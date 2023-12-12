#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QString>
#include <QJsonArray>
#include <QList>

using namespace std;

class UserAttributes {
public:
    QString userID;
    bool talkingMenu = true;
    QString menuLanguage = " ";
    QString fontName;
    int fontSize = 10;
    QString textColour;
    QString backgroundColour;
    bool sensoryFeedbackMaster = false;
    bool bluetooth = false;
    bool wifiEnabled = false;
    int volume = 100;
    int brightness = 100;
    int readbackPitch = 100;

    QJsonObject toJsonObject() const {
        return QJsonObject {
            {"userID", userID},
            {"talkingMenu", talkingMenu},
            {"menuLanguage", menuLanguage},
            {"fontName", fontName},
            {"fontSize", fontSize},
            {"textColour", textColour},
            {"backgroundColour", backgroundColour},
            {"sensoryFeedbackMaster", sensoryFeedbackMaster},
            {"bluetooth", bluetooth},
            {"wifiEnabled", wifiEnabled},
            {"volume", volume},
            {"brightness", brightness},
            {"readbackPitch", readbackPitch}
        };
    }
};


////Base Class
class User {
private:
    QString mUserId = "";
    int mUserCount = 0;
    QMap<QString, UserAttributes> mapUserAttributes;
    QJsonObject userAttributesJSON;

public:

    QList<UserAttributes> userAttributeList;
    QString getUserID() const {
        return mUserId;
    }

    int getUserCount() const {
        return mUserCount;
    }

    void setUserID(const QString &id) {
        mUserId = id;
    }

    void setUserCount(int count) {
        mUserCount = count;
    }

    void setUserAttributes(const QString &userID, const UserAttributes &attributes) {
        mapUserAttributes.insert(userID, attributes);
    }



    int getUserAttributesSize() const {
        return mapUserAttributes.size();
    }

    QMap<QString, UserAttributes> getUserAttributes() const {
        return mapUserAttributes;
    }

    void addUserAttribute(const UserAttributes &attributes) {
        userAttributeList.append(attributes);
    }

    QList<UserAttributes> getUserAttributesList() const {
        return userAttributeList;
    }


    // Update internal data only
    void updateUserAttributes(const QString &userID, const UserAttributes &updatedAttributes) {
        for (int i = 0; i < userAttributeList.size(); ++i) {
            if (userAttributeList[i].userID == userID) {
                userAttributeList[i] = updatedAttributes;
                break;
            }
        }
    }

    // Get internal data as JSON
    QJsonObject getUserAttributesAsJson() const {
        QJsonObject userAttributes;
        for (const auto &attributes : userAttributeList) {
            userAttributes[attributes.userID] = attributes.toJsonObject();
        }
        return userAttributes;
    }


//    QJsonObject getUserAttributesAsJson() const {
//        QJsonObject userAttributes;
//        for (const auto &attributes : userAttributeList) {
//            userAttributes[attributes.userID] = attributes.toJsonObject();
//        }
//        return userAttributes;
//    }

};

class Student : public User
{
private:
    QString mStudentFirstName;
    QString mStudentPassword;

public:


    Student(const QString &id, const QString &password, const QString &firstName)
        : User() {}

    QString getStudentPasscode() const {
        return mStudentPassword;
    }

    QString getStudentFirstName() const {
        return mStudentFirstName;
    }

    void setStudentPasscode(const QString &passcode) {
        mStudentPassword = passcode;
    }

    void setStudentFirstName(const QString &firstName) {
        mStudentFirstName = firstName;
    }

};

class Admin : public User
{
private:
    QString mAdminFirstName;
    QString mAdminPassword;
    QMap<int, UserAttributes> mapAdminUser; // Map to store admin attributes with integer IDs
    int adminIdCounter = 1; // Counter for admin IDs

public:

    Admin(const QString &id, const QString &password, const QString &firstName, int count)
        : User() {}

    QString getAdminPasscode() const {
        return mAdminPassword;
    }

    QString getAdminFirstName() const {
        return mAdminFirstName;
    }

    void setAdminPasscode(const QString &passcode) {
        mAdminPassword = passcode;
    }

    void setAdminFirstName(const QString &firstName) {
        mAdminFirstName = firstName;
    }


};



void saveUserAttributesToJson(const QString &filename, const QJsonObject &userAttributes) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        cerr << "Error: Can't open file for writing." << endl;
        return;
    }

    QJsonDocument jsonDoc(userAttributes);
    QByteArray data = jsonDoc.toJson();

    file.write(data); // Write the JSON data to the file
    file.close();
}


//Function for Updateing the data from the JSON file
void updateDataFromJsonFile(User* user, Student* student, Admin* admin, const QJsonObject& jsonObject)
{
    user->setUserID(jsonObject["userID"].toString());
    user->setUserCount(jsonObject["userCount"].toInt());
    student->setStudentPasscode(jsonObject["userPassword"].toString());
    student->setStudentFirstName(jsonObject["userFirstName"].toString());
    admin->setAdminPasscode(jsonObject["userPassword"].toString());
    admin->setAdminFirstName(jsonObject["userFirstName"].toString());

}

//Function for write the JSON file
void writeInJson(const QString& filename, const QJsonObject& jsonObject)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly |QIODevice::Text)) {
        cerr << "Error: Can't open file for writing." << endl;
        return;
    }

    QTextStream out(&file);
    out.seek(file.size());

    // Write the content
    QJsonDocument jsonDoc(jsonObject);
    out << jsonDoc.toJson();
    file.close();
}

//Function for read the JSON file
QJsonObject readFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        cerr << "Error: Can't open file for reading." << endl;
        return QJsonObject();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    return jsonDoc.object();
}


void displayAttributesById(const QString& id, const QList<UserAttributes>& userAttributeList)
{
    for (const auto& attributes : userAttributeList) {
        if (attributes.userID == id) {
            qDebug().noquote() << "Attributes for ID" << id << ":" << attributes.toJsonObject();
            cout << endl;
            return;
        }
    }
    qDebug().noquote() << "Attributes for ID" << id << "not found!";
}

// Function to map user attributes with IDs
QMap<QString, QJsonObject> mapUserAttributes(const QJsonObject& attributesJson) {
    QMap<QString, QJsonObject> userAttributeMap;

    for (const QString& key : attributesJson.keys()) {
        QJsonObject userAttributes = attributesJson[key].toObject();
        userAttributeMap[key] = userAttributes;
    }
    return userAttributeMap;
}

QMap<QString, QString> mapUserFirstNameWithID(const QJsonObject& userData) {
    QMap<QString, QString> userFirstNameMap;

    QStringList userTypes = { "admin" , "student" };

    for (const QString& userType : userTypes) {
        if (userData.contains(userType)) {
            QJsonArray userArray = userData[userType].toArray();
            for (const auto& user : userArray) {
                QJsonObject userObject = user.toObject();

                QString userIDKey = (userType == "admin") ? "Admin ID" : "Student ID";
                QString firstNameKey = (userType == "admin") ? "Admin FirstName" : "Student FirstName";

                if (userObject.contains(userIDKey) && userObject.contains(firstNameKey)) {
                    QString userID = userObject[userIDKey].toString();
                    QString firstName = userObject[firstNameKey].toString();
                    userFirstNameMap[firstName] = userID;
                }
            }
        }
    }

    return userFirstNameMap;
}

//Main Function
int main()
{
    User user;
    //  QList<UserAttributes> userAttributeList;

    UserAttributes adminAttributes1{
        "1234abc",
        true,
        "French",
        "Bold",
        16,
        "black",
        "white",
        false,
        false,
        true,
        100,
        90,
        100
    };

    UserAttributes adminAttributes2{
        "1234abcd",
        true,
        "French",
        "Bold",
        16,
        "black",
        "white",
        false,
        false,
        true,
        100,
        90,
        100
    };

    UserAttributes studentAttributes1{
        "4567abc",
        false,
        "English",
        "Calibri",
        15,
        "Blue",
        "Yellow",
        false,
        true,
        false,
        500,
        90,
        100
    };

    UserAttributes studentAttributes2{
        "456def",
        true,
        "English",
        "Calibri",
        20,
        "blue",
        "Beige",
        true,
        false,
        false,
        100,
        90,
        100
    };

    UserAttributes studentAttributes3{
        "456defg",
        true,
        "English",
        "Calibri",
        20,
        "blue",
        "Beige",
        true,
        false,
        false,
        100,
        90,
        100
    };

    UserAttributes studentAttributes4 {};

    user.userAttributeList.append(adminAttributes1);
    user.setUserAttributes(adminAttributes1.userID, adminAttributes1);

    user.userAttributeList.append(adminAttributes2);
    user.setUserAttributes(adminAttributes2.userID, adminAttributes2);

    user.userAttributeList.append(studentAttributes1);
    user.setUserAttributes(studentAttributes1.userID, studentAttributes1);

    user.userAttributeList.append(studentAttributes2);
    user.setUserAttributes(studentAttributes2.userID, studentAttributes2);

    user.userAttributeList.append(studentAttributes3);
    user.setUserAttributes(studentAttributes3.userID, studentAttributes3);

   // QJsonObject userAttributes;

    QList<UserAttributes> attributesList = user.getUserAttributesList();
    for (auto &attributes : attributesList) {
        if (attributes.userID == "1234abc") {
            attributes.fontSize = 20;
            // Comment out this line to only update the JSON when needed
            // user.updateUserAttributes(attributes.userID, attributes);
            break;
        }
    }

    // Save updated attributes to JSON file only when necessary
    // Extract JSON when required to be saved
    QJsonObject userAttributes = user.getUserAttributesAsJson();
    saveUserAttributesToJson("userAttributes.json", userAttributes);

    for (const auto &attr : user.getUserAttributesList())
    {
        userAttributes[attr.userID] = attr.toJsonObject();
    }

    writeInJson("userAttributes.json", userAttributes);

    //    //map inserting
    //    user.setUserAttributes("avhsg", adminAttributes1);
    //    user.setUserAttributes("hsxaj",adminAttributes2);
    //    user.setUserAttributes("aswsd",studentAttributes1);
    //    user.setUserAttributes("sadag",studentAttributes2);
    //    user.setUserAttributes("wdqgg",studentAttributes3);

    // Reading existing JSON data from file
    QJsonObject userData = readFile("users.json");

    userAttributes = readFile("userAttributes.json");

    // Mapping user first names with IDs
    QMap<QString, QString> userFirstNameIDMap = mapUserFirstNameWithID(userData);

    // Mapping user attributes with IDs
    QMap<QString, QJsonObject> userAttributeMap = mapUserAttributes(userAttributes);

    QString searchFirstName = "Ron Weasley";
    if (userFirstNameIDMap.contains(searchFirstName)) {
        QString userID = userFirstNameIDMap[searchFirstName];

        if (userAttributeMap.contains(userID)) {
            QJsonObject userAttributes = userAttributeMap[userID];
            qDebug().noquote() << "Attributes for user with FirstName" << searchFirstName << "and ID" << userID << ":" << userAttributes;
            return 0; // Exiting after finding the user
        } else {
            qDebug().noquote() << "User attributes not found for FirstName" << searchFirstName << "and ID" << userID;
        }
    } else {
        qDebug().noquote() << "User with FirstName" << searchFirstName << "not found!";
    }

    //admin JSON objects
    QJsonObject adminJson1{
        {"Admin ID", "8d9e8a7b-26d7-4149-af40-2ad3908b95c6"},
        {"Admin Password", "212345"},
        {"Admin FirstName", "Albus Dumbledore"}
    };

    QJsonObject adminJson2{
        {"Admin ID", "8d9e8a7b-26d7-4149-af40-2ad3908b95c6"},
        {"Admin Password", "212345"},
        {"Admin FirstName", "Prof. Snape"}
    };

    //student JSON objects
    QJsonObject studentJson1{
        {"Student ID", "4567abc"},
        {"Student Password", "2345"},
        {"Student FirstName", "Hermione Granger"}
    };

    QJsonObject studentJson2{
        {"Student ID", "456def"},
        {"Student Password", "5678"},
        {"Student FirstName", "Ron Weasley"}
    };

    QJsonObject studentJson3{
        {"Student ID", "422192bf-daaf-4f3c-83c2-114fc4276e85"},
        {"Student Password", "9123"},
        {"Student FirstName", "Harry Potter"}
    };
    QJsonObject studentJson4{
        {"Student ID", "422192bf-daaf-4f3c-83c2-114fc4276e85"},
        {"Student Password", "9123"},
        {"Student FirstName", "Wednesday"}
    };

    QJsonArray adminInfoArray {
        adminJson1,
        adminJson2
    };

    QJsonArray studentInfoArray {
        studentJson1,
        studentJson2,
        studentJson3,
        studentJson4,

    };
    QJsonObject userObject;

    userData["admin"] =  adminInfoArray;
    userData["student"] =  studentInfoArray;
    userObject["data"] =  userData;

    writeInJson("users.json", userData);

    QString searchID = "4567abc";
    displayAttributesById(searchID, user.userAttributeList);
    userData = readFile("users.json");

    // Check if both JSON objects are not empty
    if (userAttributes.isEmpty() || userData.isEmpty()) {
        qDebug().noquote() << "Error: Empty JSON data.";
        return 1;
    }

    // Assuming users.json has keys "admin" and "student"
    if (userData.contains("admin")) {
        QJsonArray adminArray = userData["admin"].toArray();
        for (const auto& admin : adminArray) {
            QJsonObject adminObj = admin.toObject();
            QString adminID = adminObj["Admin ID"].toString();

            // Check if userAttributes contain this admin ID
            if (userAttributeMap.contains(adminID)) {
                QJsonObject adminAttributes = userAttributeMap[adminID];
                qDebug().noquote() << "Admin ID:" << adminID << "Attributes:" << adminAttributes;
            }
        }
    }

    if (userData.contains("student")) {
        QJsonArray studentArray = userData["student"].toArray();
        for (const auto& student : studentArray) {
            QJsonObject studentObj = student.toObject();
            QString studentID = studentObj["Student ID"].toString();

            // Check if userAttributes contain this student ID
            if (userAttributeMap.contains(studentID)) {
                QJsonObject studentAttributes = userAttributeMap[studentID];
                qDebug().noquote() << "Student ID:" << studentID << "Attributes:" << studentAttributes;
            }
        }
    }


    QJsonDocument jsonDoc(userData);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug().noquote() << jsonData;
}
