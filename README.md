###Background 
Each Web server routinely logs accesses from other Web servers and browsers. The log is a text file in which each line contains a date and a hostname. Each date is logged in the format dd/mm/yyyy. 

Each hostname ends with a 2-letter country code such as uk or fr (or a 3-letter code such as com) preceded by a dot/period/full-stop (‘.’). The final token in a hostname is usually called the “top level domain”, or TLD for short. The log might look like this:

```
05/11/1999 www.intel.com 
12/12/1999 www.dcs.gla.ac.uk05/11/2001 www.mit.edu31/12/1999 www.cms.rgu.ac.uk25/12/1999 www.informatik.tum.de
```
A new FCC regulation requires that we track access by country, being able to demonstrate the percentage of accesses from each country over a given time period. The politicians have allowed that tracking accesses by TLD is sufficient to satisfy the regulation. If the period of interest is 01/08/1999 to 31/07/2000, given the above log, the output from the program should look like this:

```
33.33 com16.67 de
50.00 uk
```
###SpecificationGiven a start date, an end date, and one or more log files, the program is to determine the percentage of access from each TLD during that period, outputting the final percentages on standard output, as shown above.
Hostnames, and therefore, top level domain names, are case-insensitive. Therefore, accesses by X.Y.UK and a.b.uk are both accesses from the same TLD.