#pragma once

class MyException : public std::exception
{
public:
	MyException(int line, const std::string& file);
	~MyException() override = default;

	int GetLine() const { return line; }
	const std::string& GetFile() const { return file; }
	virtual std::string GetType() const;
	virtual std::string GetMessage() const;
	std::string GetLocationString() const;
	const char* what() const override;
private:
	int line;
	std::string file;
	mutable std::string whatString;
};
