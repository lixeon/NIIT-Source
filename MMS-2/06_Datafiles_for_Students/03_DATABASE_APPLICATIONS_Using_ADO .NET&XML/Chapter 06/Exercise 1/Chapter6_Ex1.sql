USE HR
CREATE TABLE Address_Details(AddressDetails xml)
INSERT Address_Details VALUES('<AddressDetails><EmpCode>E001</EmpCode><Address>20 Thomson Street</Address><City>Albany</City></AddressDetails>')
INSERT Address_Details VALUES('<AddressDetails><EmpCode>E002</EmpCode><Address>218 Peters Avenue</Address><City>Belmont</City><State>California</State></AddressDetails>')
SELECT * FROM Address_Details



