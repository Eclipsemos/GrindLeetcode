import java.util.*;

// Enum for book status
enum BookStatus {
    AVAILABLE, 
    CHECKED_OUT
}

// Represents a book in the library
class Book {
    private String id;
    private String title;
    private BookStatus status;

    public Book(String id, String title) {
        this.id = id;
        this.title = title;
        this.status = BookStatus.AVAILABLE;
    }

    public String getId() {
        return id;
    }

    public String getTitle() {
        return title;
    }

    public BookStatus getStatus() {
        return status;
    }

    public void checkout() {
        if (status == BookStatus.AVAILABLE) {
            status = BookStatus.CHECKED_OUT;
        }
    }

    public void checkin() {
        status = BookStatus.AVAILABLE;
    }

    @Override
    public String toString() {
        return "Book [id=" + id + ", title=" + title + ", status=" + status + "]";
    }
}

// Represents a member of the library
class Member {
    private String id;
    private String name;
    private List<Book> borrowedBooks;  // Books currently borrowed

    public Member(String id, String name) {
        this.id = id;
        this.name = name;
        borrowedBooks = new ArrayList<>();
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public List<Book> getBorrowedBooks() {
        return borrowedBooks;
    }

    public void borrowBook(Book book) {
        borrowedBooks.add(book);
    }

    public void returnBook(Book book) {
        borrowedBooks.remove(book);
    }

    @Override
    public String toString() {
        return "Member [id=" + id + ", name=" + name + "]";
    }
}

// Represents a checkout receipt
class Receipt {
    private String receiptId;
    private Member member;
    private Book book;
    private Date checkoutDate;

    public Receipt(String receiptId, Member member, Book book) {
        this.receiptId = receiptId;
        this.member = member;
        this.book = book;
        this.checkoutDate = new Date();
    }

    @Override
    public String toString() {
        return "Receipt [receiptId=" + receiptId 
                + ", member=" + member.getName() 
                + ", book=" + book.getTitle() 
                + ", checkoutDate=" + checkoutDate + "]";
    }
}

// The core Library Management System
class LibraryManagementSystem {
    private Map<String, Member> members;
    private Map<String, Book> books;
    private List<Receipt> receipts;

    public LibraryManagementSystem() {
        members = new HashMap<>();
        books = new HashMap<>();
        receipts = new ArrayList<>();
    }

    // Add a new member to the system
    public void addMember(Member member) {
        members.put(member.getId(), member);
    }

    // Add a new book to the library collection
    public void addBook(Book book) {
        books.put(book.getId(), book);
    }

    public Member getMember(String memberId) {
        return members.get(memberId);
    }

    public Book getBook(String bookId) {
        return books.get(bookId);
    }

    // Handles the checkout operation
    public Receipt checkoutBook(String memberId, String bookId) {
        Member member = getMember(memberId);
        Book book = getBook(bookId);
        
        if (member == null || book == null) {
            System.out.println("Member or Book not found.");
            return null;
        }
        
        if (book.getStatus() == BookStatus.CHECKED_OUT) {
            System.out.println("Book is already checked out.");
            return null;
        }
        
        // Proceed with checkout
        book.checkout();
        member.borrowBook(book);
        Receipt receipt = new Receipt(UUID.randomUUID().toString(), member, book);
        receipts.add(receipt);
        System.out.println("Checkout successful: " + receipt);
        return receipt;
    }

    // Handles the return operation
    public void returnBook(String memberId, String bookId) {
        Member member = getMember(memberId);
        Book book = getBook(bookId);
        
        if (member == null || book == null) {
            System.out.println("Member or Book not found.");
            return;
        }
        
        if (book.getStatus() == BookStatus.AVAILABLE) {
            System.out.println("Book is already available in the library.");
            return;
        }
        
        // Process return
        book.checkin();
        member.returnBook(book);
        System.out.println("Return successful for book: " + book.getTitle());
    }

    // Prints all checkout receipts
    public void printReceipts() {
        System.out.println("\n--- Receipts ---");
        for (Receipt r : receipts) {
            System.out.println(r);
        }
    }
}

// Main class to run the Library Management System
public class Main {
    public static void main(String[] args) {
        LibraryManagementSystem library = new LibraryManagementSystem();

        // Adding some members
        Member member1 = new Member("M001", "Alice");
        Member member2 = new Member("M002", "Bob");
        library.addMember(member1);
        library.addMember(member2);

        // Adding some books
        Book book1 = new Book("B001", "The Great Gatsby");
        Book book2 = new Book("B002", "1984");
        library.addBook(book1);
        library.addBook(book2);

        // Simulate checkout operations
        library.checkoutBook("M001", "B001");
        library.checkoutBook("M002", "B002");

        // Attempt to checkout an already checked-out book
        library.checkoutBook("M001", "B001");

        // Simulate a return operation
        library.returnBook("M001", "B001");

        // Print all checkout receipts
        library.printReceipts();
    }
}
