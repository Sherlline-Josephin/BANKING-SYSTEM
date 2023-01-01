#include<iostream> 
#include<fstream> 
#include<cstdlib> 
#include<vector> 
#include<map>

using namespace std;

#define BALANCE 0
class InsufficientFunds{};

class Account
{
	private:
		long accountNumber; 
		string firstName; 
		string lastName; 
		float balance;
		static long NextAccountNumber; 
	public:
		Account(){}
		Account(string fname,string lname,float balance); 
		long getAccNo(){return accountNumber;}
		string getFirstName(){return firstName;} 
		string getLastName(){return lastName;} 
		float getBalance(){return balance;}

		void Deposit(float amount); 
		void Withdraw(float amount);
		static void setLastAccountNumber(long accountNumber); 
		static long getLastAccountNumber();
		friend ofstream & operator<<(ofstream &ofs,Account &acc); 
		friend ifstream & operator>>(ifstream &ifs,Account &acc); 
		friend ostream & operator<<(ostream &os,Account &acc);
};
long Account::NextAccountNumber=0;

class Bank
{
	private:
	map<long,Account> accounts; 
	public:
		Bank();
		Account OpenAccount(string fname,string lname,float balance); 
		Account BalanceEnquiry(long accountNumber);
		Account Deposit(long accountNumber,float amount); 
		Account Withdraw(long accountNumber,float amount); 
		void CloseAccount(long accountNumber);
		void ShowAllAccounts();
		~Bank();
};

int main()
{
	Bank b;
	Account acc;

	int choice;
	string fname,lname;
 	long accountNumber; 
	float balance; 
	float amount;
	cout<<"\n\t\t\t\t--------------\n\t\t\t\tBANKING SYSTEM\n\t\t\t\t--------------"<<endl; 
	do
	{
		cout<<"\nSelect one option below:\n------------------------"; 
		cout<<"\n\n\t1.) Open an Account"; 
		cout<<"\n\t2.) Balance Enquiry"; 
		cout<<"\n\t3.) Deposit";
		cout<<"\n\t4.) Withdrawal"; 
		cout<<"\n\t5.) Close an Account"; 
		cout<<"\n\t6.) Show All Accounts"; 
		cout<<"\n\t7.) Quit"; 
		cout<<"\n\nEnter your choice: "; 
		cin>>choice;
		switch(choice)
		{
			case 1:
				cout<<"\nEnter First Name      : "; 
				cin>>fname;
				cout<<"Enter Last Name       : "; 
				cin>>lname;
				cout<<"Enter initial Balance : "; 
				cin>>balance; 
				acc=b.OpenAccount(fname,lname,balance);
				cout<<endl<<"\nCONGRATS...! Account is Created"<<endl; 
				cout<<acc;
				break; 
			case 2:
				cout<<"\nEnter Account Number :"; 
				cin>>accountNumber; 
				acc=b.BalanceEnquiry(accountNumber); 
				cout<<endl<<"\nYour Account Details :\n---------------------"<<endl; 
				cout<<acc;
				break; 
			case 3:
				cout<<"\nEnter Account Number : "; 
				cin>>accountNumber; 
				cout<<"Enter Balance          : "; 
				cin>>amount;
				acc=b.Deposit(accountNumber, amount); 
				cout<<endl<<"Amount is Deposited\n-------------------"<<endl; 
				cout<<acc;
				break; 
			case 4:
				cout<<"\nEnter Account Number : "; 
				cin>>accountNumber; 
				cout<<"Enter Balance        :"; 
				cin>>amount;
				acc=b.Withdraw(accountNumber, amount); 
				cout<<endl<<"Amount Withdrawn\n----------------"<<endl; 
				cout<<acc;
				break; 
			case 5:
				cout<<"\nEnter Account Number:"; 
				cin>>accountNumber; 
				b.CloseAccount(accountNumber); 
				cout<<endl<<"---Account is Closed---"<<endl; 
				//cout<<acc;
				break;
			case 6:
				cout<<"\nDisplaying All Accounts\n-----------------------\n";
				//if(b.ShowAllAccounts()==)
				//cout<<"\nNo accounts to display..";
			//	else
				b.ShowAllAccounts(); 
				break;
			case 7: break; 
			default:	 
				cout<<"\nEnter corret choice"; 
				exit(0);
		}
	}while(choice!=7);
	return 0;
}

Account::Account(string fname,string lname,float balance)
{
	NextAccountNumber++; 
	accountNumber=NextAccountNumber; 
	firstName=fname; 
	lastName=lname;
	this->balance=balance;
}
void Account::Deposit(float amount)
{
	balance+=amount;
}
void Account::Withdraw(float amount)
{
	try
	{
	if(balance-amount<BALANCE)
 		throw InsufficientFunds();
	else if(balance-=amount<500)
	{
	balance-=amount;
	cout<<"\nWARNING .!  Minimum balance is less than 500 "<<endl;
	}
	else
	balance-=amount;
	}
	catch(InsufficientFunds e)
	{
		cout<<"\nThere is no amount in the account "<<endl;
	}
}
void Account::setLastAccountNumber(long accountNumber)
{
	NextAccountNumber=accountNumber;
}
long Account::getLastAccountNumber()
{
	return NextAccountNumber;
}
ofstream & operator<<(ofstream &ofs,Account &acc)
{
	ofs<<acc.accountNumber<<endl; 
	ofs<<acc.firstName<<endl; 
	ofs<<acc.lastName<<endl; 
	ofs<<acc.balance<<endl; 
	return ofs;
}
ifstream & operator>>(ifstream &ifs,Account &acc)
{
	ifs>>acc.accountNumber; 
	ifs>>acc.firstName; 
	ifs>>acc.lastName; 
	ifs>>acc.balance; 
	return ifs;
}
ostream & operator<<(ostream &os,Account &acc)
{
	os<<"\nFirst Name     : "<<acc.getFirstName()<<endl; 
	os<<"Last Name      : "<<acc.getLastName()<<endl; 
	os<<"Account Number : "<<acc.getAccNo()<<endl; 
	os<<"Balance        : "<<acc.getBalance()<<endl; 
	return os;
}

Bank::Bank()
{

	Account account; 
	ifstream infile;
	infile.open("DATA.data"); 
	if(!infile)
	{
		cout<<"Error in Opening! File Not Found!!"<<endl; 
		return;
	}
	while(!infile.eof())
	{
		infile>>account; 
		accounts.insert(pair<long,Account>(account.getAccNo(),account));
	}
	Account::setLastAccountNumber(account.getAccNo()); 
	infile.close();
}

Account Bank::OpenAccount(string fname,string lname,float balance)
{
	ofstream outfile;
	Account account(fname,lname,balance); 
	accounts.insert(pair<long,Account>(account.getAccNo(),account));

	outfile.open("DATA.data", ios::trunc);

	map<long,Account>::iterator itr; 
	for(itr=accounts.begin();itr!=accounts.end();itr++)
	{
		outfile<<itr->second;
	}
	outfile.close(); 
	return account;
}

Account Bank::BalanceEnquiry(long accountNumber)
{
	map<long,Account>::iterator itr=accounts.find(accountNumber); 
	return itr->second;
}

Account Bank::Deposit(long accountNumber,float amount)
{
	map<long,Account>::iterator itr=accounts.find(accountNumber); 
	itr->second.Deposit(amount);
	return itr->second;
}

Account Bank::Withdraw(long accountNumber,float amount)
{
	map<long,Account>::iterator itr=accounts.find(accountNumber); 
	itr->second.Withdraw(amount);
	return itr->second;
}

void Bank::CloseAccount(long accountNumber)
{
	map<long,Account>::iterator itr=accounts.find(accountNumber); 
	cout<<"\nAccount Deleted\n"<<itr->second; 
	accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts()
{
	map<long,Account>::iterator itr;
		
	if(accounts.begin() == accounts.end()){
		cout<<"\nNo Accounts to Display..!\n\n";
	}
	else{
		for(itr=accounts.begin();itr!=accounts.end();itr++){
		cout<<"Account "<<itr->first<<"\n----------"<<endl<<itr->second<<endl;
		}
	}

}

Bank::~Bank()
{
 	ofstream outfile; 
	outfile.open("DATA.data", ios::trunc);

	map<long,Account>::iterator itr; 
	for(itr=accounts.begin();itr!=accounts.end();itr++)
	{
		outfile<<itr->second;
	}
	outfile.close();
}
