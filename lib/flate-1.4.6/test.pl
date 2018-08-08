use CGI::TemplateHTML;

my $template = new CGI::TemplateHTML();

$template->setFile("file.html");
$template->setVar("test", "toto");

$template->print;

