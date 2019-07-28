#include "../header/parser.h"
#include <stdexcept>

namespace monkey {
    std::unordered_map<TokenType, Precedence> precedences({
        {EQ,       EQUALS},
        {NE,       EQUALS},
        {LT,       LESSGREATER},
        {GT,       LESSGREATER},
        {LE,       LESSGREATER},
        {GE,       LESSGREATER},
        {PLUS,     SUM},
        {MINUS,    SUM},
        {SLASH,    PRODUCT},
        {ASTERISK, PRODUCT},
        {PERCENT,  PRODUCT},
        {LPAREN,   CALL}
    });

    void Parser::nextToken() {
        curToken = peekToken;
        peekToken = l.NextToken();
    }

    bool Parser::expectPeek(const TokenType& t) {
        if(peekToken.type == t) {
            nextToken();
            return true;
        } else {
            peekError(t);
            return false;
        }
    }

    Precedence Parser::peekPrecedence() {
        if(precedences.find(peekToken.type) != precedences.end())
            return precedences[peekToken.type];
        return LOWEST;
    }

    Precedence Parser::curPrecedence() {
        if(precedences.find(curToken.type) != precedences.end())
            return precedences[curToken.type];
        return LOWEST;
    }

    void Parser::peekError(const TokenType& t) {
        errors.push_back("expect to be " + t + ", got " + peekToken.type);
    }

    Statement* Parser::parseStatement() {
        if (curToken.type == LET) {
            return parseLetStatement();
        } else if (curToken.type == RETURN) {
            return parseReturnStatement();
        } else {
            return parseExpressionStatement();
        }
    }

    LetStatement* Parser::parseLetStatement() {
        LetStatement* stmt = new LetStatement();
        stmt->token = curToken;

        if(!expectPeek(IDENT)) {
            delete stmt;
            return nullptr;
        }
        stmt->name.token = curToken;
        stmt->name.value = curToken.literal;
        if(!expectPeek(ASSIGN)) {
            delete stmt;
            return nullptr;
        }
        nextToken();

        stmt->value = parseExpression(LOWEST);

        if (peekToken.type == SEMICOLON)
            nextToken();
        return stmt;
    }

    ReturnStatement* Parser::parseReturnStatement() {
        ReturnStatement* stmt = new ReturnStatement();
        stmt->token = curToken;

        nextToken();

        stmt->returnValue = parseExpression(LOWEST);

        if (peekToken.type == SEMICOLON)
            nextToken();
        return stmt;
    }

    ExpressionStatement* Parser::parseExpressionStatement() {
        ExpressionStatement* stmt = new ExpressionStatement();
        stmt->token = curToken;
        stmt->expression = parseExpression(LOWEST);

        if (peekToken.type == SEMICOLON)
            nextToken();
        return stmt;
    }

    BlockStatement* Parser::parseBlockStatement() {
        BlockStatement* block = new BlockStatement();
        block->token = curToken;
        
        nextToken();

        while(curToken.type != RBRACE && curToken.type != END) {
            Statement* stmt = parseStatement();
            if (stmt != nullptr) {
                block->statements.push_back(stmt);
            }
            nextToken();
        }
        return block;
    }

    Expression* Parser::parseExpression(Precedence precedence) {
        if (prefixParseFns.find(curToken.type) == prefixParseFns.end()) {
            noPrefixParseFnError(curToken.type);
            return nullptr;
        }
        PrefixParseFn prefix = prefixParseFns[curToken.type];

        Expression* leftExp = (this->*prefix)(); // notice this is the correct way to call a method pointer
        
        while(peekToken.type != SEMICOLON && precedence < peekPrecedence()) {
            if (infixParseFns.find(peekToken.type) == infixParseFns.end()) {
                return leftExp;
            }
            InfixParseFn infix = infixParseFns[peekToken.type]; 

            nextToken();

            leftExp = (this->*infix)(leftExp);
        }
        return leftExp;
    }

    Expression* Parser::parseIdentifier() {
        Identifier* ident = new Identifier();
        ident->token = curToken;
        ident->value = curToken.literal;
        return ident;
    }

    Expression* Parser::parseBoolean() {
        BooleanLiteral* ident = new BooleanLiteral();
        ident->token = curToken;
        ident->value = curToken.type == TRUE;
        return ident;
    }

    Expression* Parser::parseIntegerLiteral() {
        IntegerLiteral* num = new IntegerLiteral();
        num->token = curToken;
        try {
            num->value = std::stoi(curToken.literal);
        } catch (const std::invalid_argument e) {  // C++ does not have a base class for exception
            errors.push_back("cannot parse the " + curToken.literal + " to int");
            return nullptr;
        } catch (const std::out_of_range e) {  // this is suggested as best practice
            errors.push_back("the integer " + curToken.literal + " is out of range");
            return nullptr;
        }
        return num;
    }

    Expression* Parser::parseStringLiteral() {
        StringLiteral* s = new StringLiteral();
        s->token = curToken;
        s->value = curToken.literal;
        return s;
    }

    std::vector<Identifier*> Parser::parseFunctionParameters() {
        std::vector<Identifier*> parameters;

        if(peekToken.type == RPAREN) {  // no parameters
            nextToken();
            return parameters;
        }

        nextToken(); // pass "," or "("

        Identifier* ident = new Identifier();
        ident->token = curToken;
        ident->value = curToken.literal;
        parameters.push_back(ident);

        while(peekToken.type == COMMA) {
            nextToken();
            nextToken();
            Identifier* ident = new Identifier();
            ident->token = curToken;
            ident->value = curToken.literal;
            parameters.push_back(ident);
        }

        // TODO, deal with the error
        if(!expectPeek(RPAREN))
            return std::vector<Identifier*>();
        return parameters;
    }

    Expression* Parser::parseFunctionLiteral() {
        FunctionLiteral* fn = new FunctionLiteral();
        fn->token = curToken;

        if(!expectPeek(LPAREN)) { // in this function, only deal with "("
            delete fn;
            return nullptr;
        }

        fn->parameters = parseFunctionParameters();

        if(!expectPeek(LBRACE)) {
            delete fn;
            return nullptr;
        }

        fn->body = parseBlockStatement();
        return fn;
    }

    Expression* Parser::parsePrefixExpression() {
        PrefixExpression* exp = new PrefixExpression();
        exp->token = curToken;
        exp->op = curToken.literal;

        nextToken();

        exp->right = parseExpression(PREFIX);
        return exp;
    }

    Expression* Parser::parseGroupedExpression() {
        nextToken(); // pass "("

        Expression* exp = parseExpression(LOWEST);

        if(!expectPeek(RPAREN)) {
            delete exp;
            return nullptr;
        }

        return exp;
    }

    Expression* Parser::parseIfExpression() {
        IfExpression* exp = new IfExpression();
        exp->token = curToken;
        
        // condition
        if(!expectPeek(LPAREN)) {
            delete exp;
            return nullptr;
        }
        nextToken();
        exp->condition = parseExpression(LOWEST);
        if(!expectPeek(RPAREN)) {
            delete exp;
            return nullptr;
        }
        
        // consequence
        if(!expectPeek(LBRACE)) {
            delete exp;
            return nullptr;
        }
        exp->consequence = parseBlockStatement();

        // else
        if (peekToken.type == ELSE) {
            nextToken();
            if (!expectPeek(LBRACE)) {
                delete exp;
                return nullptr;
            }
            exp->alternative = parseBlockStatement();
        }
        return exp;
    }

    Expression* Parser::parseWhileExpression() {
        WhileExpression* exp = new WhileExpression();
        exp->token = curToken;
        
        // condition
        if(!expectPeek(LPAREN)) {
            delete exp;
            return nullptr;
        }
        nextToken();
        exp->condition = parseExpression(LOWEST);
        if(!expectPeek(RPAREN)) {
            delete exp;
            return nullptr;
        }
        
        // consequence
        if(!expectPeek(LBRACE)) {
            delete exp;
            return nullptr;
        }
        exp->consequence = parseBlockStatement();

        return exp;
    }

    std::vector<Expression*> Parser::parseCallArguments() {
        std::vector<Expression*> arguments;

        if(peekToken.type == RPAREN) {
            nextToken();
            return arguments;
        }

        nextToken();
        arguments.push_back(parseExpression(LOWEST));

        while(peekToken.type == COMMA) {
            nextToken();
            nextToken();
            arguments.push_back(parseExpression(LOWEST));
        }

        if(!expectPeek(RPAREN))
            return std::vector<Expression*>();
        return arguments;
    }
    
    Expression* Parser::parseCallExpression(Expression* function) {
        CallExpression* exp = new CallExpression();
        exp->token = curToken;
        exp->function = function;
        exp->arguments = parseCallArguments();
        return exp;
    }

    Expression* Parser::parseInfixExpression(Expression* left) {
        InfixExpression* exp = new InfixExpression();
        exp->token = curToken;
        exp->op = curToken.literal;
        exp->left = left;

        Precedence precedence = curPrecedence();
        nextToken();
        exp->right = parseExpression(precedence);

        return exp;
    }

    void Parser::noPrefixParseFnError(TokenType type) {
        errors.push_back("no prefix parse fucntion for " + type);
    }

    void Parser::New(Lexer& l) {
        this->l = l;

        // prefix parse functions
        prefixParseFns[IDENT]    = &Parser::parseIdentifier;  // this is how to assign a method pointer
        prefixParseFns[INT]      = &Parser::parseIntegerLiteral;
        prefixParseFns[STRING]   = &Parser::parseStringLiteral;
        prefixParseFns[BANG]     = &Parser::parsePrefixExpression;
        prefixParseFns[MINUS]    = &Parser::parsePrefixExpression;
        prefixParseFns[TRUE]     = &Parser::parseBoolean;
        prefixParseFns[FALSE]    = &Parser::parseBoolean;
        prefixParseFns[LPAREN]   = &Parser::parseGroupedExpression;
        prefixParseFns[IF]       = &Parser::parseIfExpression;
        prefixParseFns[WHILE]      = &Parser::parseWhileExpression;
        prefixParseFns[FUNCTION] = &Parser::parseFunctionLiteral;
        // infix parse functions
        infixParseFns[PLUS]     = &Parser::parseInfixExpression;
        infixParseFns[MINUS]    = &Parser::parseInfixExpression;
        infixParseFns[SLASH]    = &Parser::parseInfixExpression;
        infixParseFns[ASTERISK] = &Parser::parseInfixExpression;
        infixParseFns[PERCENT]  = &Parser::parseInfixExpression;
        infixParseFns[EQ]       = &Parser::parseInfixExpression;
        infixParseFns[NE]       = &Parser::parseInfixExpression;
        infixParseFns[LT]       = &Parser::parseInfixExpression;
        infixParseFns[GT]       = &Parser::parseInfixExpression;
        infixParseFns[LE]       = &Parser::parseInfixExpression;
        infixParseFns[GE]       = &Parser::parseInfixExpression;
        infixParseFns[LPAREN]   = &Parser::parseCallExpression;
        // read two tokens, so that both curToken and peekToken are set.
        nextToken();
        nextToken();
    }

    Program* Parser::ParseProgram() {
        Program *program = new Program();

        while(curToken.type != END) {
            Statement* stmt = parseStatement();
            if(stmt != nullptr) {
                program->statements.push_back(stmt);
            }
            nextToken();
        }
        return program;
    }
}