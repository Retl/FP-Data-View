#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <psapi.h>

namespace fp_data_view {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		HANDLE hFP = NULL;
		DWORD baseAddr;
		DWORD offsetPosX = 0x1DA0A70;
		DWORD offsetPosY = 0x1DA0A78;
		DWORD offsetScreenID = 0x1DD4D50;

		int screenID = -1;
		double posX = 0;
	private: System::Windows::Forms::Label^  lblScreenID;
	private: System::Windows::Forms::Label^  lblErrors;
	public:

	public:
		double posY = 0;
		//MODULEENTRY32 mFP = NULL;

		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lblPosition;
	private: System::Windows::Forms::Timer^  tmrIntervalTimer;
	private: System::ComponentModel::IContainer^  components;
	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->lblPosition = (gcnew System::Windows::Forms::Label());
			this->tmrIntervalTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->lblScreenID = (gcnew System::Windows::Forms::Label());
			this->lblErrors = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// lblPosition
			// 
			this->lblPosition->AutoSize = true;
			this->lblPosition->Location = System::Drawing::Point(32, 25);
			this->lblPosition->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->lblPosition->Name = L"lblPosition";
			this->lblPosition->Size = System::Drawing::Size(134, 25);
			this->lblPosition->TabIndex = 0;
			this->lblPosition->Text = L"_FP_POS_";
			// 
			// tmrIntervalTimer
			// 
			this->tmrIntervalTimer->Enabled = true;
			this->tmrIntervalTimer->Interval = 16;
			this->tmrIntervalTimer->Tick += gcnew System::EventHandler(this, &MyForm::tmrIntervalTimer_Tick);
			// 
			// lblScreenID
			// 
			this->lblScreenID->AutoSize = true;
			this->lblScreenID->Location = System::Drawing::Point(32, 71);
			this->lblScreenID->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->lblScreenID->Name = L"lblScreenID";
			this->lblScreenID->Size = System::Drawing::Size(150, 25);
			this->lblScreenID->TabIndex = 1;
			this->lblScreenID->Text = L"_FP_SCRN_";
			// 
			// lblErrors
			// 
			this->lblErrors->AutoSize = true;
			this->lblErrors->Location = System::Drawing::Point(30, 115);
			this->lblErrors->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->lblErrors->Name = L"lblErrors";
			this->lblErrors->Size = System::Drawing::Size(107, 25);
			this->lblErrors->TabIndex = 2;
			this->lblErrors->Text = L"ERRORS";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(15, 25);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(704, 141);
			this->Controls->Add(this->lblErrors);
			this->Controls->Add(this->lblScreenID);
			this->Controls->Add(this->lblPosition);
			this->Font = (gcnew System::Drawing::Font(L"Verdana", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(8, 6, 8, 6);
			this->Name = L"MyForm";
			this->Text = L"FP Data View";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void tmrIntervalTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
		LocateFP();
		UpdateFPState();
	}

			 System::Void LocateFP() {
				 if (hFP == NULL) {
					 this->lblPosition->Text = "Searching for Instance of FP.";
					 HANDLE phandle;

					 PROCESSENTRY32 entry;
					 MODULEENTRY32W mEntry;
					 mEntry.dwSize = sizeof(MODULEENTRY32);
					 entry.dwSize = sizeof(PROCESSENTRY32);

					 HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

					 if (Process32First(snapshot, &entry) == TRUE)
					 {
						 while (Process32Next(snapshot, &entry) == TRUE)
						 {
							 if (wcscmp(entry.szExeFile, L"FP.exe") == 0)
							 {
								 //HANDLE snap2 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, entry.th32ProcessID);
								 HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
									 PROCESS_VM_READ, FALSE, entry.th32ProcessID);
								 hFP = hProcess;
								 GetProcBaseAddr();


								 
								 this->lblPosition->Text = "Found running FP instance.";
							 }
						 }
					 }
				 }
				 else {
					 DWORD lpExitCode;
					 GetExitCodeProcess(hFP, &lpExitCode);
					 if (lpExitCode != STILL_ACTIVE) {
						 CloseHandle(hFP);
						 hFP = NULL;
					 }
				 }
			 }

			 System::Void LocateFPBad() {
				 if (hFP == NULL) {
					 this->lblPosition->Text = "Searching for Instance of FP.";
					 HANDLE phandle;

					 PROCESSENTRY32 entry;
					 MODULEENTRY32W mEntry;
					 mEntry.dwSize = sizeof(MODULEENTRY32);
					 entry.dwSize = sizeof(PROCESSENTRY32);

					 HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

					 if (Process32First(snapshot, &entry) == TRUE)
					 {
						 while (Process32Next(snapshot, &entry) == TRUE)
						 {
							 if (wcscmp(entry.szExeFile, L"FP.exe") == 0)
							 {
								 //HANDLE snap2 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, entry.th32ProcessID);
								 HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
									 PROCESS_VM_READ, FALSE, entry.th32ProcessID);
								 hFP = hProcess;
								 Module32First(hFP, &mEntry);
								 bool moduleFound = false;

								 do
								 {
									 if (wcscmp(mEntry.szModule, L"FP.exe") == 0) {
										 moduleFound = true;
										 baseAddr = (DWORD)mEntry.hModule;

									 }
									 /*
									 _tprintf(TEXT("\n\n     MODULE NAME:     %s"), me32.szModule);
									 _tprintf(TEXT("\n     executable     = %s"), me32.szExePath);
									 _tprintf(TEXT("\n     process ID     = 0x%08X"), me32.th32ProcessID);
									 _tprintf(TEXT("\n     ref count (g)  =     0x%04X"), me32.GlblcntUsage);
									 _tprintf(TEXT("\n     ref count (p)  =     0x%04X"), me32.ProccntUsage);
									 _tprintf(TEXT("\n     base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
									 _tprintf(TEXT("\n     base size      = %d"), me32.modBaseSize);
									 */

								 } while (!moduleFound && Module32Next(hFP, &mEntry));

								 // baseAddr = (DWORD) mEntry.hModule;
								 // baseAddr = (DWORD) mEntry.modBaseAddr;
								 this->lblPosition->Text = "Found running FP instance.";

								 /*
								 DWORD WINAPI GetModuleFileNameEx(
								 __in      HANDLE hProcess,
								 __in_opt  HMODULE hModule,
								 __out     LPTSTR lpFilename,
								 __in      DWORD nSize
								 );
								 */

							 }
						 }
					 }
				 }
				 else {
					 DWORD lpExitCode;
					 GetExitCodeProcess(hFP, &lpExitCode);
					 if (lpExitCode != STILL_ACTIVE) {
						 CloseHandle(hFP);
						 hFP = NULL;
					 }
				 }
			 }

			 
			 System::Void GetProcBaseAddr() {
				 HMODULE hMods[2048];
				 DWORD cbNeeded;
				 int retries = 0;
				 int i = 0;

				 // Enumerate the Modules
				 // Get a list of all the modules in this process.
				 if (EnumProcessModules(hFP, hMods, sizeof(hMods), &cbNeeded) != 0)
				 {
					 for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
					 {
						 TCHAR szModName[MAX_PATH];

						 // Get the full path to the module's file.

						 if (GetModuleFileNameEx(hFP, hMods[i], szModName,
							 sizeof(szModName) / sizeof(TCHAR)))
						 {
							 // Print the module name and handle value.

							 //_tprintf(TEXT("\t%s (0x%08X)\n"), szModName, hMods[i]);
						 }
					 }
					 baseAddr = (DWORD)hMods[0]; // Module Base for FP.
					 //cout << "New Start Address: " << startAddress;
					 //address = startAddress + offsetToPosition;
					 retries = -1;
				 }
				 else
				 {
					 retries += 1;
					 //cout << "Error when attempting to Enumerate Modules. Error code: " << GetLastError() << "\n";
					 //cout << "Retrying. Num Retries: " << retries << "\n";
				 }
			 }
			 

			 System::Void UpdateFPState() {
				 if (hFP != NULL) {
					 /*
					 ReadDoubleFromMem(offsetPosX, &posX);
					 ReadDoubleFromMem(offsetPosY, &posY);
					 ReadIntFromMem(offsetScreenID, &screenID);
					 */
					 posX = ReadDoubleFromMem(offsetPosX, posX);
					 posY = ReadDoubleFromMem(offsetPosY, posY);
					 screenID = ReadIntFromMem(offsetScreenID, screenID);

					 if (screenID < -5 || screenID > 100) {
						 hFP = NULL;
						 LocateFP();
						 return;
					 }
					 
					 this->lblPosition->Text = "Position: ( " + posX + ", " + posY + " )";
					 this->lblScreenID->Text = "Screen ID: ( " + screenID + " )";
				 }
			 }

			 /*
			 System::Void ReadDoubleFromMem(DWORD offsetAddr, void *value) {
				 if (hFP != NULL) {
					 if (ReadProcessMemory(hFP, (LPCVOID*)(baseAddr + offsetAddr), value, sizeof(double), NULL) != 0)
					 {

					 }
				 }
			 }
			 */

			 int ReadIntFromMem(DWORD offsetAddr, int value) {
				 int result = value;
				 if (hFP != NULL) {
					 if (ReadProcessMemory(hFP, (LPCVOID*)(baseAddr + offsetAddr), &result, sizeof(result), NULL) != 0)
					 {
						 this->lblErrors->Visible = false;
						 return result;
					 }
					 else
					 {
						 this->lblErrors->Visible = true;
						 this->lblErrors->Text = ("Last Error: " + GetLastError());
					 }
				 }
				 return value;
			 }

			 double ReadDoubleFromMem(DWORD offsetAddr, double value) {
				 double result = value;
				 if (hFP != NULL) {
					 if (ReadProcessMemory(hFP, (LPCVOID*)(baseAddr + offsetAddr), &result, sizeof(result), NULL) != 0)
					 {
						 this->lblErrors->Visible = false;
						 return result;
					 }
					 else
					 {
						 this->lblErrors->Visible = true;
						 this->lblErrors->Text = ("Last Error: " + GetLastError());
					 }
				 }
				 return value;
			 }

	};
}
