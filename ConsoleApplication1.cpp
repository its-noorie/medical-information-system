#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

// Utility Functions for Input Validation
string getValidatedInput(const string& prompt, const regex& pattern, const string& errorMessage) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (regex_match(input, pattern)) {
            return input; // Valid input
        }
        else {
            cout << errorMessage << endl;
        }
    }
}

int getValidatedInteger(const string& prompt, const string& errorMessage) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        try {
            int value = stoi(input);
            if (value > 0) return value; // Valid positive integer
        }
        catch (...) {
            // Catch invalid integer conversions
        }
        cout << errorMessage << endl;
    }
}

// Admin Login Function
bool adminLogin() {
    char username[20], password[20];

    for (int attempts = 0; attempts < 3; attempts++) {
        cout << "Enter Admin Username: ";
        cin >> username;

        if (string(username) != "admin") {
            cout << "Invalid username! Attempts left: " << (2 - attempts) << "\n";
            continue;
        }

        cout << "Enter Admin Password: ";
        cin >> password;

        if (string(password) != "123") {
            cout << "Invalid password! Attempts left: " << (2 - attempts) << "\n";
            continue;
        }

        return true;
    }

    cout << "Too many failed attempts! Exiting...\n";
    return false;
}

int getCurrentYear() {
    time_t t = time(0);  // Get current time
    struct tm now;       // Create a tm struct to hold local time

    // Use localtime_s for safer local time conversion
    localtime_s(&now, &t);

    return now.tm_year + 1900; // tm_year gives years since 1900
}

bool validateDate(const string& date) {
    regex datePattern(R"((\d{4})-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|30|31))");
    if (!regex_match(date, datePattern)) return false;

    stringstream ss(date);
    string yearStr, monthStr, dayStr;
    getline(ss, yearStr, '-');
    getline(ss, monthStr, '-');
    getline(ss, dayStr, '-');

    int year = stoi(yearStr);
    int month = stoi(monthStr);
    int day = stoi(dayStr);

    if (year != getCurrentYear()) return false;

    // Validate days in each month
    if (month == 2) return day <= 28; // February
    if (month == 4 || month == 6 || month == 9 || month == 11) return day <= 30; // Months with 30 days
    return day <= 31; // Months with 31 days
}

bool validateID(const string& filename, int id) {
    ifstream file(filename);
    if (!file) return false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        getline(ss, field, ','); // Read ID field
        if (stoi(field) == id) return true;
    }
    return false;
}

// Patient Class
class Patient {
public:
    int id;
    string name;
    int age;
    string email;
    string phone_no;
    string medicalHistory;
    string vitals; // Ensure this is defined

    Patient() : id(0), age(0) {}

    Patient(int id, const string& name, int age, const string& email, const string& phone_no, const string& medicalHistory, const string& vitals = "")
        : id(id), name(name), age(age), email(email), phone_no(phone_no), medicalHistory(medicalHistory), vitals(vitals) {}

    string serialize() const {
        // Ensure no extra comma if vitals are empty
        if (vitals.empty()) {
            return to_string(id) + "," + name + "," + to_string(age) + "," + email + "," + phone_no + "," + medicalHistory;
        }
        return to_string(id) + "," + name + "," + to_string(age) + "," + email + "," + phone_no + "," + medicalHistory + "," + vitals;
    }

    static Patient deserialize(const string& data) {
        stringstream ss(data);
        string field;
        Patient p;

        getline(ss, field, ','); p.id = stoi(field);
        getline(ss, p.name, ',');
        getline(ss, field, ','); p.age = stoi(field);
        getline(ss, p.email, ',');
        getline(ss, p.phone_no, ',');
        getline(ss, p.medicalHistory, ',');

        // If vitals exist, deserialize them as well
        if (getline(ss, p.vitals, '.')) {
            // Vitals are present
        }

        return p;
    }

    void display() const {
        // Display vitals only if available
        cout << "ID: " << id << ", Name: " << name
            << ", Age: " << age << ", Email: " << email
            << ", History: " << medicalHistory;

        if (!vitals.empty()) {
            cout << ", Vitals: " << vitals;  // Display full vitals properly
        }
        cout << endl;
    }


    void updateVitals(const string& newTemperature, const string& newBloodPressure, const string& newHeartRate, int nurseID) {
        // Default values for vitals
        string temp = "N/A", bp = "N/A", hr = "N/A";

        // If new values are provided, update them
        if (!newTemperature.empty()) temp = "Temp: " + newTemperature + "°C";
        if (!newBloodPressure.empty()) bp = "BP: " + newBloodPressure;
        if (!newHeartRate.empty()) hr = "HR: " + newHeartRate + " bpm";

        // Add Nurse ID who recorded the vitals
        string nurseInfo = "(Recorded by Nurse ID: " + to_string(nurseID) + ")";

        // Combine the updated components into a single vitals string
        vitals = temp + ", " + bp + ", " + hr + " " + nurseInfo;
    }
};


class NursingStaff {
public:
    int id;
    string name;
    string shift;

    NursingStaff() : id(0) {}

    NursingStaff(int id, const string& name, const string& shift)
        : id(id), name(name), shift(shift) {}

    string serialize() const {
        return to_string(id) + "," + name + "," + shift;
    }

    static NursingStaff deserialize(const string& data) {
        stringstream ss(data);
        string field;
        NursingStaff n;

        getline(ss, field, ','); n.id = stoi(field);
        getline(ss, n.name, ',');
        getline(ss, n.shift, ',');

        return n;
    }

    void display() const {
        cout << "ID: " << id << ", Name: " << name
            << ", Shift: " << shift << endl;
    }
};



// Doctor Class
class Doctor {
public:
    int id;
    string name;
    string specialization;

    Doctor() : id(0) {}

    Doctor(int id, const string& name, const string& specialization)
        : id(id), name(name), specialization(specialization) {}

    string serialize() const {
        return to_string(id) + "," + name + "," + specialization;
    }

    static Doctor deserialize(const string& data) {
        stringstream ss(data);
        string field;
        Doctor d;

        getline(ss, field, ','); d.id = stoi(field);
        getline(ss, d.name, ',');
        getline(ss, d.specialization, ',');

        return d;
    }

    void display() const {
        cout << "ID: " << id << ", Name: " << name
            << ", Specialization: " << specialization << endl;
    }
};

// Appointment Class
class Appointment {
public:
    int id;
    int patientID;
    int doctorID;
    string date;
    string time;

    Appointment() : id(0), patientID(0), doctorID(0) {}

    Appointment(int id, int patientID, int doctorID, const string& date, string& time)
        : id(id), patientID(patientID), doctorID(doctorID), date(date) , time(time){}

    string serialize() const {
        return to_string(id) + "," + to_string(patientID) + "," + to_string(doctorID) + "," + date + "," + time;
    }

    static Appointment deserialize(const string& data) {
        stringstream ss(data);
        string field;
        Appointment a;

        getline(ss, field, ','); a.id = stoi(field);
        getline(ss, field, ','); a.patientID = stoi(field);
        getline(ss, field, ','); a.doctorID = stoi(field);
        getline(ss, a.date, ',');
        getline(ss, a.time, ',');
        return a;
    }

    void display() const {
        cout << "ID: " << id << ", Patient ID: " << patientID
            << ", Doctor ID: " << doctorID << ", Date: " << date << ", Time: " <<endl;
    }
};

// MIS Class
class MIS {
private:
    const string PATIENT_FILE = "patients.txt";
    const string DOCTOR_FILE = "doctors.txt";
    const string NURSING_FILE = "nursing_staff.txt";
    const string APPOINTMENT_FILE = "appointments.txt";


    int getNextID(const string& filename) {
        ifstream file(filename);
        if (!file) return 1;

        string line, lastLine;
        while (getline(file, line)) {
            lastLine = line;
        }
        file.close();

        if (lastLine.empty()) return 1;

        stringstream ss(lastLine);
        string idStr;
        getline(ss, idStr, ',');
        return stoi(idStr) + 1;
    }

    void appendToFile(const string& filename, const string& data) {
        ofstream file(filename, ios::app);
        if (!file) {
            cerr << "Error: Could not open file for writing.\n";
            return;
        }
        file << data << endl;
        file.close();
    }

public:
    // Add Patient
    void addPatient() {
        int id = getNextID(PATIENT_FILE);

        regex emailPattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");  // Email validation pattern
        regex phonePattern(R"(\+?\d{1,3}[\s-]?\(?\d{1,4}\)?[\s-]?\d{1,4}[\s-]?\d{1,4}[\s-]?\d{1,4})"); // Phone validation pattern

        cout << "Enter Patient Name: ";
        string name;
        getline(cin, name);

        int age = getValidatedInteger("Enter Age: ", "Invalid age. Please enter a valid positive integer.");
        string email = getValidatedInput("Enter Email (e.g., abc@abc.com): ", emailPattern, "Invalid email. Please enter a valid email address.");

        // Ask for phone number and validate it
        string phone = getValidatedInput("Enter Phone Number (e.g., +921234567890): ", phonePattern, "Invalid phone number. Please enter a valid phone number.");

        cout << "Enter Medical History: ";
        string history;
        getline(cin, history);

        Patient p(id, name, age, email, phone, history);
        appendToFile(PATIENT_FILE, p.serialize());
        cout << "Patient added successfully! Assigned ID: " << id << endl;
    }


    // Add Doctor
    void addDoctor() {
        int id = getNextID(DOCTOR_FILE);

        regex namePattern(R"([A-Za-z\s]+)");

        string name = getValidatedInput("Enter Doctor Name: ", namePattern, "Invalid name. Only letters and spaces are allowed.");
        string specialization = getValidatedInput("Enter Specialization: ", namePattern, "Invalid specialization. Only letters and spaces are allowed.");

        Doctor d(id, name, specialization);
        appendToFile(DOCTOR_FILE, d.serialize());
        cout << "Doctor added successfully! Assigned ID: " << id << endl;
    }
    // Update the Add Nursing Staff Function
    void addNursingStaff() {
        int id = getNextID(NURSING_FILE);

        // Updated name pattern to disallow numbers
        regex namePattern(R"([A-Za-z\s]+)");  // Only allow letters and spaces

        cout << "Enter Nursing Staff Name: ";
        string name;
        getline(cin, name);

        // Ensure name follows the pattern
        while (!regex_match(name, namePattern)) {
            cout << "Invalid name. Only letters and spaces are allowed." << endl;
            cout << "Enter Nursing Staff Name: ";
            getline(cin, name);
        }

        // Updated shift validation to only accept valid shifts
        regex shiftPattern(R"(^(Morning|Evening|Night)$)"); // Valid shifts only: Morning, Evening, or Night
        string shift;

        cout << "Enter Shift (Morning, Evening, Night): ";
        getline(cin, shift);

        while (!regex_match(shift, shiftPattern)) {
            cout << "Invalid shift. Please enter 'Morning', 'Evening', or 'Night'." << endl;
            cout << "Enter Shift (Morning, Evening, Night): ";
            getline(cin, shift);
        }

        NursingStaff n(id, name, shift);
        appendToFile(NURSING_FILE, n.serialize());
        cout << "Nursing staff added successfully! Assigned ID: " << id << endl;
    }

    void bookAppointment() {
        int id = getNextID(APPOINTMENT_FILE);
        int patientID = getValidatedInteger("Enter Patient ID: ", "Invalid Patient ID. Please enter a positive integer.");
        int doctorID = getValidatedInteger("Enter Doctor ID: ", "Invalid Doctor ID. Please enter a positive integer.");

        string date = getValidatedInput("Enter Appointment Date (YYYY-MM-DD): ", regex(R"(\d{4}-\d{2}-\d{2})"), "Invalid date format. Use YYYY-MM-DD.");
        while (!validateDate(date)) {
            cout << "Invalid date. Ensure it's this year and correct month/day limits." << endl;
            date = getValidatedInput("Enter Appointment Date (YYYY-MM-DD): ", regex(R"(\d{4}-\d{2}-\d{2})"), "Invalid date format.");
        }

        string time = getValidatedInput("Enter Appointment Time (HH:MM): ", regex(R"(^(?:[01]\d|2[0-3]):[0-5]\d$)"), "Invalid time. Use 24-hour HH:MM format.");

        Appointment a(id, patientID, doctorID, date, time);
        appendToFile(APPOINTMENT_FILE, a.serialize());
        cout << "Appointment booked successfully! Assigned ID: " << id << endl;
    }

    void addVitals() {
        int patientID = getValidatedInteger("Enter Patient ID for recording vitals: ", "Invalid ID. Please enter a positive integer.");

        if (!validateID("patients.txt", patientID)) {
            cout << "Patient with ID " << patientID << " not found.\n";
            return;
        }

        int nurseID = getValidatedInteger("Enter Nurse ID recording the vitals: ", "Invalid Nurse ID. Please enter a positive integer.");
        if (!validateID("nursing_staff.txt", nurseID)) {
            cout << "Nurse with ID " << nurseID << " not found.\n";
            return;
        }

        regex tempPattern(R"(\d+(\.\d{1,2})?)");
        regex bpPattern(R"(\d{1,3}/\d{1,3})");
        regex heartratePattern(R"(\d{1,3})");

        string temperature = getValidatedInput("Enter Temperature (°C): ", tempPattern, "Invalid temperature. Enter a numeric value.");
        string bloodPressure = getValidatedInput("Enter Blood Pressure (e.g., 120/80): ", bpPattern, "Invalid blood pressure. Format: 120/80.");
        string heartRate = getValidatedInput("Enter Heart Rate (bpm): ", heartratePattern, "Invalid heart rate. Enter a positive integer.");

        ifstream file("patients.txt");
        ofstream tempFile("temp_patients.txt");

        string line;
        while (getline(file, line)) {
            Patient p = Patient::deserialize(line);
            if (p.id == patientID) {
                p.updateVitals(temperature, bloodPressure, heartRate, nurseID);  // Update vitals here
                cout << "Vitals updated for Patient ID " << patientID << endl;
            }
            tempFile << p.serialize() << endl;
        }

        file.close();
        tempFile.close();

        // Rename operation with error handling
        if (remove("patients.txt") != 0) {
            perror("Error deleting original file");
        }
        else if (rename("temp_patients.txt", "patients.txt") != 0) {
            perror("Error renaming file");
        }
        else {
            cout << "Patient file successfully updated.\n";
        }
    }




    // View Appointments
    void viewAppointments() {
        cout << "\n--- Appointments ---\n";
        ifstream file(APPOINTMENT_FILE);
        if (!file) {
            cout << "No appointment data available.\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            Appointment a = Appointment::deserialize(line);
            a.display();
        }
        file.close();
    }

    // View Patients
    void viewPatients() {
        cout << "\n--- Patients ---\n";
        ifstream file(PATIENT_FILE);
        if (!file) {
            cout << "No patient data available.\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            Patient p = Patient::deserialize(line);
            p.display();
        }
        file.close();
    }

    // View Doctors
    void viewDoctors() {
        cout << "\n--- Doctors ---\n";
        ifstream file(DOCTOR_FILE);
        if (!file) {
            cout << "No doctor data available.\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            Doctor d = Doctor::deserialize(line);
            d.display();
        }
        file.close();
    }

    // View Nursing Staff
    void viewNursingStaff() {
        cout << "\n--- Nursing Staff ---\n";
        ifstream file(NURSING_FILE);
        if (!file) {
            cout << "No nursing staff data available.\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            NursingStaff n = NursingStaff::deserialize(line);
            n.display();
        }
        file.close();
    }
};

// Main Function
int main() {
    cout << R"(  __  __          _ _           _   _____        __                           _   _                _____           _                 
 |  \/  |        | (_)         | | |_   _|      / _|                         | | (_)              / ____|         | |                
 | \  / | ___  __| |_  ___ __ _| |   | |  _ __ | |_ ___  _ __ _ __ ___   __ _| |_ _  ___  _ __   | (___  _   _ ___| |_ ___ _ __ ___  
 | |\/| |/ _ \/ _` | |/ __/ _` | |   | | | '_ \|  _/ _ \| '__| '_ ` _ \ / _` | __| |/ _ \| '_ \   \___ \| | | / __| __/ _ \ '_ ` _ \ 
 | |  | |  __/ (_| | | (_| (_| | |  _| |_| | | | || (_) | |  | | | | | | (_| | |_| | (_) | | | |  ____) | |_| \__ \ ||  __/ | | | | |
 |_|  |_|\___|\__,_|_|\___\__,_|_| |_____|_| |_|_| \___/|_|  |_| |_| |_|\__,_|\__|_|\___/|_| |_| |_____/ \__, |___/\__\___|_| |_| |_|
                                                                                                          __/ |                      
                                                                                                         |___/                       )";
    cout << endl;
    // Admin login check
    if (!adminLogin()) {
        return 0; // Exit if login fails
    }

    MIS system;
    int choice=0;

    while (choice!=10) {
        cout << "\n--- Medical Information System Menu ---\n";
        cout << "1. Add Patient\n2. View Patients\n3. Add Doctor\n4. View Doctors\n5. Add Nursing Staff\n6. View Nursing Staff\n7. Add Patient Vitals\n8. Book Appointment\n9. View Appointments\n10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            system.addPatient();
            break;
        case 2:
            system.viewPatients();
            break;
        case 3:
            system.addDoctor();
            break;
        case 4:
            system.viewDoctors();
            break;
        case 5:
            system.addNursingStaff();
            break;
        case 6:
            system.viewNursingStaff();
            break;
        case 7:
            system.addVitals();
            break;
        case 8:
            system.bookAppointment();
            break;
        case 9:
            system.viewAppointments();
            break;
        case 10:
            cout << "Exiting system. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}
