#include &lt;iostream&gt;
#include &lt;algorithm&gt;
#include "base.hpp"

using namespace std;

// Implement date class with stream input and comparison operators
struct date
{
    int year, month, day;
    date() = default;
    date(int y, int m, int d) : year(y), month(m), day(d) {}
    
    // Stream input operator
    friend istream& operator&gt;&gt;(istream& is, date& d) {
        is &gt;&gt; d.year &gt;&gt; d.month &gt;&gt; d.day;
        return is;
    }
    
    // Comparison operator for dates
    bool operator&lt;(const date& other) const {
        if (year != other.year) return year &lt; other.year;
        if (month != other.month) return month &lt; other.month;
        return day &lt; other.day;
    }
};

class mail : public object
{
protected:
    string postmark;
    date send_date;
    date arrive_date;

public:
    mail() = default;

    mail(string _contain_, string _postmark_, date send_d, date arrive_d) : object(_contain_)
    {
        postmark = _postmark_;
        send_date = send_d;
        arrive_date = arrive_d;
    }

    // Override send_status function
    string send_status(int y, int m, int d) override
    {
        return "not send";
    }

    // Override type function
    string type() override
    {
        return "no type";
    }

    // Override print function
    void print() override
    {
        // Call base class print function
        object::print();
        cout &lt;&lt; "[mail] postmark: " &lt;&lt; postmark &lt;&lt; endl;
    }

    // Copy function implementation
    void copy(object *o) override
    {
        contain = reinterpret_cast&lt;mail *&gt;(o)-&gt;contain;
        postmark = reinterpret_cast&lt;mail *&gt;(o)-&gt;postmark;
        send_date = reinterpret_cast&lt;mail *&gt;(o)-&gt;send_date;
        arrive_date = reinterpret_cast&lt;mail *&gt;(o)-&gt;arrive_date;
    }
};

class air_mail : public mail
{
protected:
    string airlines;
    date take_off_date;
    date land_date;

public:
    air_mail() = default;

    air_mail(string _contain_, string _postmark_, date send_d, date arrive_d, date take_off, date land, string _airline) : mail(_contain_, _postmark_, send_d, arrive_d)
    {
        take_off_date = take_off;
        land_date = land;
        airlines = _airline;
    }

    // Override send_status function
    string send_status(int y, int m, int d) override
    {
        date ask_date(y, m, d);
        if (ask_date &lt; send_date)
            return "mail not send";
        else if (ask_date &lt; take_off_date)
            return "wait in airport";
        else if (ask_date &lt; land_date)
            return "in flight";
        else if (ask_date &lt; arrive_date)
            return "already land";
        else
            return "already arrive";
    }

    // Override type function
    string type() override
    {
        return "air";
    }

    // Override print function
    void print() override
    {
        // Call base class print function
        mail::print();
        cout &lt;&lt; "[air] airlines: " &lt;&lt; airlines &lt;&lt; endl;
    }

    // Copy function implementation
    void copy(object *o) override
    {
        // Call base class copy
        mail::copy(o);
        air_mail* other = reinterpret_cast&lt;air_mail *&gt;(o);
        take_off_date = other-&gt;take_off_date;
        land_date = other-&gt;land_date;
        airlines = other-&gt;airlines;
    }
};

class train_mail : public mail
{
protected:
    string *station_name;
    date *station_time;
    int len;

public:
    train_mail() = default;

    train_mail(string _contain_, string _postmark_, date send_d, date arrive_d, string *sname, date *stime, int station_num) : mail(_contain_, _postmark_, send_d, arrive_d)
    {
        len = station_num;
        station_name = new string[len];
        station_time = new date[len];
        for (int i = 0; i &lt; len; i++) {
            station_name[i] = sname[i];
            station_time[i] = stime[i];
        }
    }

    // Destructor to prevent memory leaks
    ~train_mail() override {
        delete[] station_name;
        delete[] station_time;
    }

    // Override send_status function
    string send_status(int y, int m, int d) override
    {
        date ask_date(y, m, d);
        if (ask_date &lt; send_date)
            return "mail not send";
        else if (ask_date &lt; station_time[0])
            return "wait in station";
        else if (ask_date &lt; station_time[len-1]) {
            // Find current station
            for (int i = 0; i &lt; len-1; i++) {
                if (ask_date &lt; station_time[i+1])
                    return "in train, next station: " + station_name[i+1];
            }
        }
        else if (ask_date &lt; arrive_date)
            return "already arrive at final station";
        else
            return "already arrive";
        return "already arrive"; // Default case
    }

    // Override type function
    string type() override
    {
        return "train";
    }

    // Override print function
    void print() override
    {
        // Call base class print function
        mail::print();
        cout &lt;&lt; "[train] station_num: " &lt;&lt; len &lt;&lt; endl;
    }

    // Copy function implementation
    void copy(object *o) override
    {
        // Call base class copy
        mail::copy(o);
        train_mail* other = reinterpret_cast&lt;train_mail *&gt;(o);
        len = other-&gt;len;
        // Allocate new memory
        station_name = new string[len];
        station_time = new date[len];
        for (int i = 0; i &lt; len; i++) {
            station_name[i] = other-&gt;station_name[i];
            station_time[i] = other-&gt;station_time[i];
        }
    }
};

class car_mail : public mail
{
protected:
    int total_mile;
    string driver;

public:
    car_mail() = default;

    car_mail(string _contain_, string _postmark_, date send_d, date arrive_d, int mile, string _driver) : mail(_contain_, _postmark_, send_d, arrive_d)
    {
        total_mile = mile;
        driver = _driver;
    }

    // Override send_status function
    string send_status(int y, int m, int d) override
    {
        date ask_date(y, m, d);
        if (ask_date &lt; send_date)
            return "mail not send";
        else if (ask_date &lt; arrive_date) {
            // Calculate total time in days (using the 360-day year system)
            int total_time = (arrive_date.year - send_date.year) * 360 + 
                           (arrive_date.month - send_date.month) * 30 + 
                           (arrive_date.day - send_date.day);
            int elapsed_time = (ask_date.year - send_date.year) * 360 + 
                             (ask_date.month - send_date.month) * 30 + 
                             (ask_date.day - send_date.day);
            
            // Calculate current mile using double for precision
            double current_mile = static_cast&lt;double&gt;(elapsed_time) / total_time * total_mile;
            return to_string(current_mile);
        }
        else
            return "already arrive";
    }

    // Override type function
    string type() override
    {
        return "car";
    }

    // Override print function
    void print() override
    {
        // Call base class print function
        mail::print();
        cout &lt;&lt; "[car] driver_name: " &lt;&lt; driver &lt;&lt; endl;
    }

    // Copy function implementation
    void copy(object *o) override
    {
        // Call base class copy
        mail::copy(o);
        car_mail* other = reinterpret_cast&lt;car_mail *&gt;(o);
        total_mile = other-&gt;total_mile;
        driver = other-&gt;driver;
    }
};

// Implementation of obj_swap function
void obj_swap(object *&amp;lhs, object *&amp;rhs) {
    object* temp = lhs;
    lhs = rhs;
    rhs = temp;
}